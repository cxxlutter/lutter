namespace lutter {

template <typename T>
quaternion<T>::quaternion()
  : r(), v() {}

template <typename T>
quaternion<T>::quaternion(T r, vector3d<T> v)
  : r(r), v(std::move(v)) {}

template <typename T>
quaternion<T>::quaternion(vector3d<T> v)
  : r(), v(std::move(v)) {}

template <typename T>
T& quaternion<T>::real() { return r; }

template <typename T>
T const& quaternion<T>::real() const { return r; }

template <typename T>
vector3d<T>& quaternion<T>::imag() { return v; }

template <typename T>
vector3d<T> const& quaternion<T>::imag() const { return v; }

template <typename T>
quaternion<T> operator*(quaternion<T> const& lhs, quaternion<T> const& rhs) {
  return quaternion<T>(lhs.real()*rhs.real() - dot(lhs.imag(), rhs.imag()),
                       lhs.real()*rhs.imag() + rhs.real()*lhs.imag() +
                       cross(lhs.imag(), rhs.imag()));
}

template <typename T>
quaternion<T>& quaternion<T>::operator+=(quaternion const& rhs) {
  r += rhs.r;
  v += rhs.v;
  return *this;
}

template <typename T>
quaternion<T>& quaternion<T>::operator-=(quaternion const& rhs) {
  r -= rhs.r;
  v -= rhs.v;
  return *this;
}

template <typename T>
quaternion<T>& quaternion<T>::operator*=(T const& scalar) {
  r *= scalar;
  v *= scalar;
  return *this;
}

template <typename T>
quaternion<T>& quaternion<T>::operator/=(T const& scalar) {
  r /= scalar;
  v /= scalar;
  return *this;
}

template <typename T>
quaternion<T> quaternion_rotation_from_units(vector3d<T> const& from,
                                             vector3d<T> const& to) {
  return normalized(quaternion<T>(1 + dot(from, to),
                                  cross(from, to)));
}
template <typename T>
quaternion<T> quaternion_rotation(vector3d<T> const& from,
                                  vector3d<T> const& to) {
  return quaternion_rotation_from_units(normalized(from),
                                        normalized(to));
}

}
