#ifndef LUTTER_QUATERNION_HPP
#define LUTTER_QUATERNION_HPP

#include <cassert>
#include <utility>
#include "vector.hpp"

namespace lutter {

template <typename T>
struct quaternion;

template <typename T>
quaternion<T> operator*(quaternion<T> const& lhs, quaternion<T> const& rhs);

template <typename T>
class quaternion {
  T r;
  vector3d<T> v;
public:
  typedef T value_type;

  quaternion();
  quaternion(T r, vector3d<T> v);
  explicit quaternion(vector3d<T> v);

  T& real();
  T const& real() const;
  vector3d<T>& imag();
  vector3d<T> const& imag() const;

  quaternion& operator+=(quaternion const& rhs);
  quaternion& operator-=(quaternion const& rhs);
  quaternion& operator*=(T const& scalar);
  quaternion& operator/=(T const& scalar);

  friend quaternion operator+(quaternion lhs, quaternion const& rhs) { return lhs += rhs; }
  friend quaternion operator-(quaternion lhs, quaternion const& rhs) { return lhs -= rhs; }
  friend quaternion operator*(T const& scalar, quaternion v) { return v *= scalar; }
  friend quaternion operator*(quaternion v, T const& scalar) { return scalar*v; }
  friend quaternion operator/(quaternion v, T const& scalar) { return v /= scalar; }

  quaternion& operator*=(quaternion const& rhs) { return *this = (*this)*rhs; }
  friend vector3d<T> operator*(quaternion const& q, vector3d<T> const& v) { return (q*quaternion(v)*conj(q)).v; }

  friend T dot(quaternion const& lhs, quaternion const& rhs) { return lhs.r*rhs.r + dot(lhs.v, rhs.v); }
  friend quaternion conj(quaternion const& q) { return quaternion(q.r, -q.v); }
};

template <typename T>
quaternion<T> quaternion_rotation_from_units(vector3d<T> const& from,
                                             vector3d<T> const& to);
template <typename T>
quaternion<T> quaternion_rotation(vector3d<T> const& from,
                                  vector3d<T> const& to);

} // end namespace lutter

#include "bits/quaternion.inl"

#endif /* LUTTER_QUATERNION_HPP */
