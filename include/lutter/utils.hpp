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
