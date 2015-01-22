// This file is part of lutter, a simple ray tracer
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LUTTER_UTILS_HPP
#define LUTTER_UTILS_HPP

#include <cassert>

namespace lutter {

#define LUTTER_UNREACHABLE __builtin_unreachable

template <typename T>
T squared(T x);

template <typename T>
T clamp(T x, T min, T max);

template <typename T, typename Scalar>
T lerp(T const& x, T const& y, Scalar t);

} // end namespace lutter

#include "bits/utils.inl"

#endif /* LUTTER_UTILS_HPP */
