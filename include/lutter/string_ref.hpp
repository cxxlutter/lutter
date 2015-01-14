#ifndef LUTTER_STRING_REF_HPP
#define LUTTER_STRING_REF_HPP

#include <cstdint>
#include <cassert>
#include <cstring>
#include <algorithm>

namespace lutter {

template <typename T>
class basic_string_ref {
  const T *ptr;
  std::size_t n;
public:
  basic_string_ref(const T *c_str, std::size_t length)
    : ptr(c_str), n(length) {}

  basic_string_ref(const T *c_str) {
    assert(c_str != nullptr);
    ptr = c_str;
    n = std::strlen(c_str);
  }

  basic_string_ref(const T *first, const T *last)
    : ptr(c_str), n(strlen(c_str)) {}

  basic_string_ref(std::basic_string<T>& str)
    : ptr(str.c_str()), n(str.size()) {}

  const T *c_str() const { return ptr; }
  std::size_t size() const { return n; }

  const T* begin() { return ptr; }
  const T* end() { return ptr + n; }

  friend bool operator==(basic_string_ref lhs, basic_string_ref rhs) {
    return (lhs.size() == rhs.size() &&
            std::equal(lhs.begin(), lhs.end(), rhs.begin()));
  }
};

using string_ref = basic_string_ref<char>;

} // end namespace lutter

#endif /* LUTTER_STRING_REF_HPP */
