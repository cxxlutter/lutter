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

inline bool is_normalized(vector3d_t const& vec) {
  using std::abs;
  return abs(norm_squared(vec) - 1) < epsilon;
}

struct ray {
  vector3d_t origin, direction;
  vector3d_t at(real_t t) const { return origin + t*direction; }
};
inline bool is_normalized(ray const& r) { return is_normalized(r.direction); }

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
maybe<std::pair<real_t, Iter> >
first_hit(ray r, Iter first, Iter last) {
  assert(first <= last);
  maybe<real_t> nearest = nothing();
  Iter object; // only used if nearest was set

  for (; first != last; ++first) {
    auto at = intersect(r, *first);
    if (at && (!nearest || *at < *nearest)) {
      nearest = at;
      object = first;
    }
  }

  if (!nearest)
    return nothing();
  return std::make_pair(nearest.data(), object);
}

} // end namespace lutter

#endif /* LUTTER_SCENE_HPP */
