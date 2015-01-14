namespace lutter {

template <typename T>
T squared(T x) {
  return x*x;
}

template <typename T>
T clamp(T x, T min, T max) {
  assert(min <= max);
  if (x > max) return max;
  if (x < min) return min;
  return x;
}

template <typename T, typename Scalar>
T lerp(T const& x, T const& y, Scalar t) {
  return x*(1-t) + y*t;
}

} // end namespace lutter
