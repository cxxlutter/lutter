// This file is part of lutter, a simple ray tracer
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LUTTER_SCENE_HPP
#define LUTTER_SCENE_HPP

#include <array>
#include <cassert>
#include <utility>
#include <cmath>
#include "vector.hpp"
#include "maybe.hpp"
#include "config.hpp"

namespace lutter {

bool is_normalized(vector3d_t const& vec);

struct ray {
  vector3d_t origin, direction;
  vector3d_t at(real_t t) const;
};
bool is_normalized(ray const& r);

struct material {
  color_t emission;
  color_t color;
};

struct sphere {
  vector3d_t center;
  real_t radius;
  material mat;
};

maybe<real_t> intersect(ray r, sphere const& s);

struct camera {
  vector3d_t location;
  vector3d_t direction;
  vector3d_t right, up;
};

struct scene {
  camera cam;
  color_t sky_emission, ground_emission;

  size_t image_width, image_height;

  // separate spheres from other objects to reduce overhead
  std::vector<sphere> spheres;
};

template <typename Iter>
maybe<std::pair<real_t, Iter> > first_hit(ray r, Iter first, Iter last);

} // end namespace lutter

#include "bits/scene.inl"

#endif /* LUTTER_SCENE_HPP */
