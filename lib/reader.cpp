// This file is part of lutter, a simple ray tracer
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <functional>
#include <istream>
#include <map>
#include <string>
#include "scene.hpp"
#include "reader.hpp"

namespace lutter {

namespace {

template <char C> // Werner Salomon's Helferlein
std::istream& Char( std::istream& in ) {
  char c;
  if (in >> c && c != C)
    in.setstate(std::ios_base::failbit);
  return in;
}

class keyword {
  std::string s;
public:
  keyword(std::string const& s) : s(s) {}
  friend std::istream& operator>>(std::istream& in, keyword const& kw) {
    if (kw.s.empty())
      return in;
    std::string s;
    if (in >> s && s != kw.s)
      in.setstate(std::ios_base::failbit);
    return in;
  }
};

struct bracket_scope {
  std::istream& in;
  bracket_scope(std::istream& in)
    : in(in) { in >> Char<'{'>; }
  ~bracket_scope() { in >> Char<'}'>; }
};

using reader_function = std::function<void(std::istream& in, scene& s)>;

struct unique_keyword {
  bool already_called = false;
  reader_function f;
  unique_keyword(reader_function f) : f(std::move(f)) {}
  void operator()(std::istream& in, scene& s) {
    if (already_called) {
      in.setstate(std::ios_base::failbit);
      return;
    }
    already_called = true;
    f(in, s);
  }
};

template <typename T>
std::istream& read_vector3d(std::istream& in, T& v, std::string kw={}) {
  return in >> keyword(kw) >> Char<'<'> >> v[0] >> Char<','> >> v[1] >> Char<','> >> v[2]
            >> Char<'>'>;
}

std::istream& read_color(std::istream& in, color_t& c, std::string kw={}) {
  read_vector3d(in, c, kw);
  c[3] = 1;
  return in;
}

void read_cam(std::istream& in, scene& s) {
  bracket_scope _(in);
  read_vector3d(in, s.cam.location, "location");
  read_vector3d(in, s.cam.direction, "direction");
  read_vector3d(in, s.cam.right, "right");
  read_vector3d(in, s.cam.up, "up");
  s.cam.direction = normalized(s.cam.direction);

  read_color(in, s.sky_emission, "sky_emission");
  read_color(in, s.ground_emission, "ground_emission");
}

void read_sphere(std::istream& in, scene& s) {
  sphere sp;
  {
    bracket_scope _(in);
    read_vector3d(in, sp.center);
    in >> Char<','> >> sp.radius;
    read_color(in, sp.mat.color, "color");
    read_color(in, sp.mat.emission, "emission");
  }
  if (in) {
    s.spheres.push_back(sp);
  }
}

void read_global(std::istream& in, scene& s) {
  bracket_scope _(in);
  in >> keyword("width") >> s.image_width
     >> keyword("height") >> s.image_height;
}

struct scene_reader {
  std::map<std::string, reader_function> keywords;

  scene_reader() {
    keywords["camera"] = unique_keyword(read_cam);
    keywords["global"] = unique_keyword(read_global);
    keywords["sphere"] = read_sphere;
  }

  bool read(std::istream& in, scene& s) {
    std::string keyword;
    while (in >> keyword) {
      if (keywords.count(keyword))
        keywords[keyword](in, s);
      else
        in.setstate(std::ios_base::failbit);
    }
    if (in.eof())
      return true;
    if (!in.bad()) {
      std::cerr << "reading failed just before those lines:";
      in.clear();
      for (int i=0; i<3; ++i) {
        std::string line;
        std::getline(in, line);
        std::cerr << line << '\n';
      }
    }
    return false;
  }
};

} // end anonymous namespace

bool read_pov_scene(std::istream& in, scene& s)
{
  scene_reader sr;
  return sr.read(in, s);
}

} // end namespace lutter
