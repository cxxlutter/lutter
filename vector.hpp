#ifndef LUTTER_VECTOR_HPP
#define LUTTER_VECTOR_HPP

#include <cstddef>
#include <cassert>
#include <utility>
#include <type_traits>
#include <iostream>
#include <algorithm>

namespace lutter {

template<bool...B> struct all_true
  : std::is_same<all_true<true, B...>, all_true<B..., true>> {};

template <typename T, std::size_t Dim>
struct vector : std::array<T, Dim> {
  vector() =default;

  template <typename... Args,
            typename = typename std::enable_if<
              (sizeof...(Args)==Dim) &&
               all_true<std::is_convertible<Args, T>::value...>::value>::type>
  vector(Args&&... args)
    : std::array<T, Dim>{{std::forward<Args>(args)...}} {}

  vector operator-() const { return T(-1)*(*this); }
  vector& operator+=(vector const& rhs) {
    for (std::size_t i=0; i<Dim; ++i)
      (*this)[i] += rhs[i];
    return *this;
  }
  vector& operator-=(vector const& rhs) {
    for (std::size_t i=0; i<Dim; ++i)
      (*this)[i] -= rhs[i];
    return *this;
  }
  vector& operator*=(T const& scalar) {
    for (auto& component : *this)
      component *= scalar;
    return *this;
  }
  vector& operator/=(T const& scalar) {
    for (auto& component : *this)
      component /= scalar;
    return *this;
  }
  friend vector operator+(vector lhs, vector const& rhs) {
    return lhs += rhs;
  }
  friend vector operator-(vector lhs, vector const& rhs) {
    return lhs -= rhs;
  }
  friend vector operator*(T const& scalar, vector v) {
    return v *= scalar;
  }
  friend vector operator*(vector v, T const& scalar) {
    return scalar*v;
  }
  friend vector operator/(vector v, T const& scalar) {
    return v /= scalar;
  }
  friend std::ostream& operator<<(std::ostream& os, vector const& v) {
    if (v.size()==0)
      return os << "<>";
    os << "<" << v[0];
    for (std::size_t i=1; i<v.size(); ++i)
      os << "," << v[i];
    return os << ">";
  }
  friend vector component_wise_multiplication(vector lhs, vector const& rhs) {
    for (std::size_t i=0; i<Dim; ++i)
      lhs[i] *= rhs[i];
    return lhs;
  }
};

template <typename T>
using vector3d = vector<T, 3>;

template <typename T>
using vector4d = vector<T, 4>;

template <typename Vec>
typename Vec::value_type dot(Vec const& lhs, Vec const& rhs) {
  using std::begin; using std::endl;
  assert(lhs.size() == rhs.size());
  return std::inner_product(begin(lhs), end(lhs), begin(rhs),
                            typename Vec::value_type{});
}

template <typename Vec>
typename Vec::value_type norm_squared(Vec const& v) {
  return dot(v, v);
}

template <typename Vec>
typename Vec::value_type abs(Vec const& v) {
  using std::sqrt;
  return sqrt(norm_squared(v));
}

template <typename Vec>
Vec normalized(Vec const& v) {
  return v/abs(v);
}

template <typename Vec>
Vec cross(Vec const& lhs, Vec const& rhs) {
  static_assert(lhs.size() == 3, "cross: arguments not in R^3");
  return Vec{
    lhs[1]*rhs[2] - lhs[2]*rhs[1],
    lhs[2]*rhs[0] - lhs[0]*rhs[2],
    lhs[0]*rhs[1] - lhs[1]*rhs[0],
  };
}

template <typename T> T squared(T x) { return x*x; }

} // end namespace lutter

#endif /* LUTTER_VECTOR_HPP */
