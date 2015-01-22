// This file is part of lutter, a simple ray tracer
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LUTTER_VECTOR_HPP
#define LUTTER_VECTOR_HPP

#include <cstddef>
#include <cassert>
#include <utility>
#include <type_traits>
#include <iostream>
#include <algorithm>

namespace lutter {

template <typename T, std::size_t Dim> struct vector;

namespace detail {

template<bool...B> struct all_true
  : std::is_same<all_true<true, B...>, all_true<B..., true>> {};

template <typename T, std::size_t Dim>
vector<T, Dim> compwise_mult(vector<T, Dim> lhs, vector<T, Dim> const& rhs);

} // end namespace detail

template <typename T, std::size_t Dim>
struct vector : std::array<T, Dim> {
  using value_type = typename std::array<T, Dim>::value_type;
  static constexpr std::size_t dimension = Dim;

  vector() =default;

  template <typename... Args,
            typename = typename std::enable_if<
              sizeof...(Args) == Dim &&
              detail::all_true<std::is_convertible<Args, T>::value...>::value
            >::type>
  vector(Args&&... args);

  vector operator-() const;
  vector& operator+=(vector const& rhs);
  vector& operator-=(vector const& rhs);
  vector& operator*=(T const& scalar);
  vector& operator/=(T const& scalar);

  friend vector operator+(vector lhs, vector const& rhs) { return lhs += rhs; }
  friend vector operator-(vector lhs, vector const& rhs) { return lhs -= rhs; }
  friend vector operator*(T const& scalar, vector v) { return v *= scalar; }
  friend vector operator*(vector v, T const& scalar) { return scalar*v; }
  friend vector operator/(vector v, T const& scalar) { return v /= scalar; }
  friend vector operator*(vector lhs, vector const& rhs) {
    return detail::compwise_mult(std::move(lhs), rhs);
  }
};

template <typename T, std::size_t Dim>
std::ostream& operator<<(std::ostream& os, vector<T, Dim> const& v);

template <typename T>
using vector3d = vector<T, 3>;

template <typename T>
using vector4d = vector<T, 4>;

template <typename Vec>
typename Vec::value_type dot(Vec const& lhs, Vec const& rhs);

template <typename Vec>
typename Vec::value_type length_squared(Vec const& v);

template <typename Vec>
typename Vec::value_type abs(Vec const& v);

template <typename Vec>
Vec normalized(Vec const& v);

template <typename Vec>
Vec cross(Vec const& lhs, Vec const& rhs);

} // end namespace lutter

#include "bits/vector.inl"

#endif /* LUTTER_VECTOR_HPP */
