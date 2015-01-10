#ifndef LUTTER_QUATERNION_HPP
#define LUTTER_QUATERNION_HPP

#include <cassert>
#include "vector.hpp"

namespace lutter {

template <typename T>
struct quaternion {
  typedef T value_type;
  T r;
  vector3d<T> v;
  quaternion() : r(), v() {}
  quaternion(T r, vector3d<T> v) : r(r), v(std::move(v)) {}
  explicit quaternion(vector3d<T> v) : r(), v(std::move(v)) {}

  quaternion& operator+=(quaternion const& rhs) {
    r += rhs.r;
    v += rhs.v;
    return *this;
  }
  quaternion& operator-=(quaternion const& rhs) {
    r -= rhs.r;
    v -= rhs.v;
    return *this;
  }
  quaternion& operator*=(T const& scalar) {
    r *= scalar;
    v *= scalar;
    return *this;
  }
  quaternion& operator/=(T const& scalar) {
    r /= scalar;
    v /= scalar;
    return *this;
  }
  friend quaternion operator+(quaternion lhs, quaternion const& rhs) {
    return lhs += rhs;
  }
  friend quaternion operator-(quaternion lhs, quaternion const& rhs) {
    return lhs -= rhs;
  }
  friend quaternion operator*(T const& scalar, quaternion v) {
    return v *= scalar;
  }
  friend quaternion operator*(quaternion v, T const& scalar) {
    return scalar*v;
  }
  friend quaternion operator/(quaternion v, T const& scalar) {
    return v /= scalar;
  }

  quaternion& operator*=(quaternion const& rhs) {
    return *this = (*this)*rhs;
  }
  friend quaternion operator*(quaternion lhs, quaternion rhs) {
    return quaternion(lhs.r*rhs.r - dot(lhs.v, rhs.v),
                      lhs.r*rhs.v + rhs.r*lhs.v + cross(lhs.v, rhs.v));
  }
  friend vector3d<T> operator*(quaternion const& q, vector3d<T> const& v) {
    return (q*quaternion(v)*conj(q)).v;
  }
  friend T dot(quaternion const& lhs, quaternion const& rhs) {
    return lhs.r*rhs.r + dot(lhs.v, rhs.v);
  }
  friend quaternion conj(quaternion const& q) {
    return quaternion(q.r, -q.v);
  }
};
template <typename T>
quaternion<T> quaternion_rotation_from_units(vector3d<T> const& from,
                                             vector3d<T> const& to) {
  return normalized(quaternion<T>(1 + dot(from, to),
                                  cross(from, to)));
}
template <typename T>
quaternion<T> quaternion_rotation(vector3d<T> from, vector3d<T> to) {
  return quaternion_rotation_from_units(normalized(from),
                                        normalized(to));
}

} // end namespace lutter

#endif /* LUTTER_QUATERNION_HPP */
