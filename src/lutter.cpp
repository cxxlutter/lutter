// This file is part of lutter, a simple ray tracer
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>
#include <string>
#include "lutter/scene.hpp"
#include "lutter/reader.hpp"
#include "lutter/vector.hpp"
#include "lutter/array2d.hpp"
#include "lutter/image.hpp"
#include "lutter/quaternion.hpp"
#include "lutter/config.hpp"

namespace lutter {

using image_t = array2d<color_t>;

color_t raytrace(scene& s, ray r, int depth=0) {
  if (depth > 32) return {};

  auto hit = first_hit(r, &*s.spheres.begin(), &*s.spheres.end());
  if (hit) {
    auto on = r.at(hit->first);
    auto& sph = *hit->second;
    auto norm = (sph.center - on)/sph.radius;
    assert(is_normalized(norm));
    auto next_ray = ray{on, normalized(r.direction-2*norm*dot(norm, r.direction))};
    assert(is_normalized(next_ray));
    auto incoming = sph.mat.color * raytrace(s, next_ray, depth+1);
    return sph.mat.emission + incoming;
  }
  if (r.direction[1] > 0)
    return s.sky_emission;
  else
    return s.ground_emission;
}

void render(scene& s, image_t& img) {
  int rendered=0, all=img.size(), last_percent=0;

  auto const ez = vector3d_t(0.,0.,1.);
  auto const cam_rot = quaternion_rotation(ez, s.cam.direction);
  assert(length_squared(cam_rot*ez - s.cam.direction) < epsilon);

  auto const dx = s.cam.right/img.width();
  auto const dy = s.cam.up/img.height();
  auto const dz = ez;

  for (size_t y=0; y<img.height(); ++y){
    for (size_t x=0; x<img.width(); ++x){
      int percent = 10*static_cast<int>(10.*++rendered/all);
      if (percent > last_percent) {
        std::cerr << "rendering " << percent << "% complete ...\n";
        last_percent = percent;
      }
      vector3d_t dir =(( real_t(x) - img.width()/real_t(2))*dx +
                       (-real_t(y) + img.height()/real_t(2))*dy +
                       dz);
      ray r{s.cam.location, normalized(cam_rot*dir)};
      img[y][x] = raytrace(s, r);
    }
  }
}

} // end namespace lutter

int main(int argc, char const* argv[]) {
  lutter::scene s;

  if (!lutter::read_pov_scene(std::cin, s))
    return 1;

  lutter::image_type format = lutter::image_type::p3;
  bool failed = false;
  for (int i=1; i<argc; ++i) {
    if (argv[i] == lutter::string_ref("-f") && i+1<argc) {
      ++i;
      if (auto f = lutter::image_type_from_string(argv[i])) {
        format = *f;
      } else {
        failed = true;
        std::cerr << "unrecognized format: `" << argv[i] << "'\n";
      }
    } else {
      failed = true;
      std::cerr << "unrecognized command line option: `" << argv[i] << "'\n";
    }
  }
  if (failed)
    return 2;

  lutter::image_t arr(s.image_height, s.image_width);
  lutter::render(s, arr);
  lutter::save_image(std::cout, arr, format);
}
