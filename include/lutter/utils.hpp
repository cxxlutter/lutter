#ifndef LUTTER_UTILS_HPP
#define LUTTER_UTILS_HPP

#include <cassert>

namespace lutter {

#define LUTTER_UNREACHABLE __builtin_unreachable

template <typename T>
T squared(T x);

template <typename T>
T clamp(T x, T min, T max);

} // end namespace lutter

#include "bits/utils.hpp"

#endif /* LUTTER_UTILS_HPP */
