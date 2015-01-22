// This file is part of lutter, a simple ray tracer
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LUTTER_ARRAY2D_HPP
#define LUTTER_ARRAY2D_HPP

#include <cstddef>
#include <vector>

namespace lutter {

namespace detail {
template <typename Array, typename ValueT> struct index_access_proxy;
} // end namespace detail

template <typename T>
class array2d {
  std::vector<T> data;
  std::size_t width_, height_;
public:
  array2d(std::size_t height, std::size_t width, T val=T());

  using value_type = T;
  using size_type = std::size_t;

  std::size_t width() const { return width_; }
  std::size_t height() const { return height_; }
  std::size_t size() const { return data.size(); }

  value_type const& unsafe_at(std::size_t y, std::size_t x) const;
  value_type& unsafe_at(std::size_t y, std::size_t x);

  detail::index_access_proxy<array2d, value_type&>
  operator[](std::size_t y);
  detail::index_access_proxy<array2d const, value_type const&>
  operator[](std::size_t y) const;
};

} // end namespace lutter

#include "bits/array2d.inl"

#endif /* LUTTER_ARRAY2D_HPP */
