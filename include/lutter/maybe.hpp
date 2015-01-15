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
  maybe(nothing_t);
  template <typename... Args>
  maybe(just_t, Args&&... args);
  maybe(T&& value);
  maybe(T const& value);
  maybe(maybe&& o);
  maybe(maybe const& o);
  maybe& operator=(maybe o);
  ~maybe();

  operator bool() const;
  bool operator!() const;

  T& data();
  T const& data() const;

  T* operator->();
  T const* operator->() const;
  T& operator*();
  T const& operator*() const;

  T get(T default_) const;
};

nothing_t nothing();

template <typename T, typename... Args>
maybe<T> just(Args&&... args);

} // end namespace lutter

#include "bits/maybe.inl"

#endif /* LUTTER_MAYBE_HPP */
