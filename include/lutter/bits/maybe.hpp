namespace lutter {

template <typename T>
maybe<T>::maybe(nothing_t)
  : has_value(false) {}

template <typename T>
template <typename... Args>
maybe<T>::maybe(just_t, Args&&... args)
  : has_value(true) {
  new (&storage) T(std::forward<Args>(args)...);
}

template <typename T>
maybe<T>::maybe(T&& value) : maybe(just_t{}, std::move(value)) {}

template <typename T>
maybe<T>::maybe(T const& value) : maybe(just_t{}, value) {}

template <typename T>
maybe<T>::maybe(maybe&& o)
  : has_value(o.has_value) { if (o) data() = std::move(o.data()); }

template <typename T>
maybe<T>::maybe(maybe const& o)
  : has_value(o.has_value) { if (o) data() = o.data(); }

template <typename T>
maybe<T>& maybe<T>::operator=(maybe o) {
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

template <typename T>
maybe<T>::~maybe() { if (*this) data().~T(); }

template <typename T>
maybe<T>::operator bool() const { return has_value; }

template <typename T>
bool maybe<T>::operator!() const { return !static_cast<bool>(*this); }

template <typename T>
T const& maybe<T>::data() const { assert(*this); return *reinterpret_cast<const T*>(&storage); }

template <typename T>
T& maybe<T>::data() { return const_cast<T&>(static_cast<maybe const&>(*this).data()); }

template <typename T>
T* maybe<T>::operator->() { return &data(); }

template <typename T>
T const* maybe<T>::operator->() const { return &data(); }

template <typename T>
T& maybe<T>::operator*() { return data(); }

template <typename T>
T const& maybe<T>::operator*() const { return data(); }

template <typename T>
T maybe<T>::get(T default_) const { return *this ? data() : default_; }

inline nothing_t nothing() { return nothing_t{}; }

template <typename T, typename... Args>
maybe<T> just(Args&&... args) {
  return maybe<T>(just_t{}, std::forward<Args>(args)...);
}

} // end namespace lutter
