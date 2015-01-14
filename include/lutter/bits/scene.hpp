namespace lutter {

inline bool is_normalized(vector3d_t const& vec) {
  using std::abs;
  return abs(length_squared(vec) - 1) < epsilon;
}

inline vector3d_t ray::at(real_t t) const {
  return origin + t*direction;
}

inline bool is_normalized(ray const& r) {
  return is_normalized(r.direction);
}

template <typename Iter>
maybe<std::pair<real_t, Iter> >
inline first_hit(ray r, Iter first, Iter last) {
  assert(first <= last);
  maybe<real_t> nearest = nothing();
  Iter object; // only used if nearest was set

  for (; first != last; ++first) {
    auto at = intersect(r, *first);
    if (at && (!nearest || *at < *nearest)) {
      nearest = at;
      object = first;
    }
  }

  if (!nearest)
    return nothing();
  return std::make_pair(nearest.data(), object);
}

} // end namespace lutter
