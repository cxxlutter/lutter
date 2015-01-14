namespace lutter {

template <typename T>
array2d<T>::array2d(std::size_t height, std::size_t width, T val)
  : data(width*height, val), width_(width), height_(height) {}

template <typename T>
auto array2d<T>::unsafe_at(std::size_t y, std::size_t x) const
  -> value_type const& {
  assert(x < width());
  assert(y < height());
  return data[y*width() + x];
}

template <typename T>
auto array2d<T>::unsafe_at(std::size_t y, std::size_t x) -> value_type& {
  auto const* cthis = this;
  return const_cast<value_type&>(cthis->unsafe_at(y, x));
}

namespace detail {

template <typename Array, typename ValueT>
struct index_access_proxy {
  Array *owner;
  std::size_t y;
  ValueT operator[](std::size_t x) {
    return owner->unsafe_at(y, x);
  }
};

} // end namespace detail

template <typename T>
auto array2d<T>::operator[](std::size_t y)
  -> detail::index_access_proxy<array2d, value_type&> {
  return {this, y};
}

template <typename T>
auto array2d<T>::operator[](std::size_t y) const
  -> detail::index_access_proxy<array2d const, value_type const&> {
  return {this, y};
}

} // end namespace lutter
