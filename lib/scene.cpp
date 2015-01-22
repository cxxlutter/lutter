// This file is part of lutter, a simple ray tracer
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>
#include <cmath>
#include "scene.hpp"
#include "config.hpp"
#include "vector.hpp"
#include "utils.hpp"

namespace lutter {

maybe<real_t> intersect(ray r, sphere const& s) {
  if (!is_normalized(r.direction))
    std::cerr << r.direction << std::endl;
  assert(is_normalized(r.direction));

  // Equation (notation: <a,b>=dot(a,b), |x|^2=norm_squared(x)):
  // (|l|^2)*t^2 + (2*<l,o-c>)*t + (|o-c|^2 - r^2) = 0
  // where l=r.direction, o=r.origin, c=s.center, r=s.radius
  // note that |l|^2=1

  const real_t a = 1;
  const auto delta = r.origin - s.center;
  const real_t b_half = dot(r.direction, delta);
  const real_t c = length_squared(delta) - squared(s.radius);

  const real_t discriminant_over_4 = squared(b_half) - a*c;

  if (discriminant_over_4 < 0)
    return nothing();
  auto sqrt_of_discriminant_half = std::sqrt(discriminant_over_4);
  auto x1 = (-b_half - sqrt_of_discriminant_half)/a;
  if (x1 > epsilon) return x1;
  auto x2 = (-b_half + sqrt_of_discriminant_half)/a;
  if (x2 > epsilon) return x1;

  return nothing();
}

} // end namespace lutter
