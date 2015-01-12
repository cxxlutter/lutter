#ifndef LUTTER_MAYBE_HPP
#define LUTTER_MAYBE_HPP

#include <cassert>
#include <utility>
#include <type_traits>

namespace lutter {

struct nothing_t {};
struct just_t {};

template <typename T>
class maybe {
  bool has_value;
  typename std::aligned_storage<sizeof(T), alignof(T)>::type storage;
public:
  maybe(nothing_t) : has_value(false) {}
  template <typename... Args>
  maybe(just_t, Args&&... args) : has_value(true) {
    new (&storage) T(std::forward<Args>(args)...);
  }
  maybe(T&& value) : maybe(just_t{}, std::move(value)) {}
  maybe(T const& value) : maybe(just_t{}, value) {}
  maybe(maybe&& o)
    : has_value(o.has_value) { if (o) data() = std::move(o.data()); }
  maybe(maybe const& o)
    : has_value(o.has_value) { if (o) data() = o.data(); }
  maybe& operator=(maybe o) {
    if (o)
      if (*this)
        data() = std::move(o.data());
      else
        new (&storage) T(std::move(o.data()));
    else if (*this)
      data().~T();
    has_value = o.has_value;
    return *this;
  }
  ~maybe() { if (*this) data().~T(); }

  operator bool() const { return has_value; }
  bool operator!() const { return !static_cast<bool>(*this); }
  T const& data() const { assert(*this); return *reinterpret_cast<const T*>(&storage); }
  T& data() { return const_cast<T&>(static_cast<maybe const&>(*this).data()); }

  T* operator->() { return &data(); }
  T const* operator->() const { return &data(); }
  T& operator*() { return data(); }
  T const& operator*() const { return data(); }

  T get(T default_) const { return *this ? data() : default_; }
};

inline nothing_t nothing() { return nothing_t{}; }
template <typename T, typename... Args>
maybe<T> just(Args&&... args) {
  return maybe<T>(just_t{}, std::forward<Args>(args)...);
}

} // end namespace lutter

#endif /* LUTTER_MAYBE_HPP */
