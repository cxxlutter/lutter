#ifndef LUTTER_ARRAY2D_HPP
#define LUTTER_ARRAY2D_HPP

#include <vector>

namespace lutter {

template <typename T>
class array2d {
  std::vector<T> data;
  size_t width_, height_;
public:
  array2d(size_t height, size_t width, T val=T())
    : data(width*height, val), width_(width), height_(height) {}

  using value_type = T;
  using size_type = size_t;

  size_t width() const { return width_; }
  size_t height() const { return height_; }
  size_t size() const { return data.size(); }

  value_type const& unsafe_at(size_t y, size_t x) const {
    assert(x < width());
    assert(y < height());
    return data[y*width() + x];
  }
  value_type& unsafe_at(size_t y, size_t x) {
    auto const* cthis = this;
    return const_cast<value_type&>(cthis->unsafe_at(y, x));
  }

  template <typename Array, typename ValueT>
  struct index_access_proxy {
    Array *owner;
    size_t y;
    ValueT operator[](size_t x) { return owner->unsafe_at(y, x); }
  };
  index_access_proxy<array2d, value_type&> operator[](size_t y) {
    return {this, y};
  }
  index_access_proxy<array2d const, value_type const&> operator[](size_t y) const {
    return {this, y};
  }
};

} // end namespace lutter

#endif /* LUTTER_ARRAY2D_HPP */
