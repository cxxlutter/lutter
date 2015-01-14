namespace lutter {

template <typename T, std::size_t Dim>
template <typename... Args, typename>
vector<T, Dim>::vector(Args&&... args)
  : std::array<T, Dim>{{std::forward<Args>(args)...}} {}

template <typename T, std::size_t Dim>
vector<T, Dim> vector<T, Dim>::operator-() const {
  vector res;
  for (std::size_t i=0; i<Dim; ++i)
    res[i] = -(*this)[i];
  return res;
}

template <typename T, std::size_t Dim>
vector<T, Dim>& vector<T, Dim>::operator+=(vector const& rhs) {
  for (std::size_t i=0; i<Dim; ++i)
    (*this)[i] += rhs[i];
  return *this;
}

template <typename T, std::size_t Dim>
vector<T, Dim>& vector<T, Dim>::operator-=(vector const& rhs) {
  for (std::size_t i=0; i<Dim; ++i)
    (*this)[i] -= rhs[i];
  return *this;
}

template <typename T, std::size_t Dim>
vector<T, Dim>& vector<T, Dim>::operator*=(T const& scalar) {
  for (auto& component : *this)
    component *= scalar;
  return *this;
}

template <typename T, std::size_t Dim>
vector<T, Dim>& vector<T, Dim>::operator/=(T const& scalar) {
  for (auto& component : *this)
    component /= scalar;
  return *this;
}

template <typename T, std::size_t Dim>
std::ostream& operator<<(std::ostream& os, vector<T, Dim> const& v) {
  if (v.size()==0)
    return os << "<>";
  os << "<" << v[0];
  for (std::size_t i=1; i<v.size(); ++i)
    os << "," << v[i];
  return os << ">";
}

namespace detail {

template <typename T, std::size_t Dim>
vector<T, Dim> compwise_mult(vector<T, Dim> lhs, vector<T, Dim> const& rhs) {
  for (std::size_t i=0; i<Dim; ++i)
    lhs[i] *= rhs[i];
  return lhs;
}

} // end namespace detail

template <typename Vec>
typename Vec::value_type dot(Vec const& lhs, Vec const& rhs) {
  using std::begin; using std::endl;
  assert(lhs.size() == rhs.size());
  return std::inner_product(begin(lhs), end(lhs), begin(rhs),
                            typename Vec::value_type{});
}

template <typename Vec>
typename Vec::value_type norm_squared(Vec const& v) {
  return dot(v, v);
}

template <typename Vec>
typename Vec::value_type abs(Vec const& v) {
  using std::sqrt;
  return sqrt(norm_squared(v));
}

template <typename Vec>
Vec normalized(Vec const& v) {
  return v/abs(v);
}

template <typename Vec>
Vec cross(Vec const& lhs, Vec const& rhs) {
  static_assert(Vec::dimension == 3, "cross: arguments not in F^3");
  return Vec{
    lhs[1]*rhs[2] - lhs[2]*rhs[1],
    lhs[2]*rhs[0] - lhs[0]*rhs[2],
    lhs[0]*rhs[1] - lhs[1]*rhs[0],
  };
}

}
