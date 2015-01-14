#ifndef LUTTER_CONFIG_HPP
#define LUTTER_CONFIG_HPP

#include "vector.hpp"

namespace lutter {

template <typename T>
using color = vector<T, 3>;

using real_t = double;
using color_t = color<real_t>;
const real_t epsilon = 1e-9;
const real_t pi = std::acos(real_t(-1));
using vector3d_t = vector3d<real_t>;
using vector4d_t = vector4d<real_t>;

} // end namespace lutter

#endif /* LUTTER_CONFIG_HPP */
