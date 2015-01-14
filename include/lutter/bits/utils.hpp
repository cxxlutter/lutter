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

} // end namespace lutter
