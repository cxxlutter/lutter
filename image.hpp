#ifndef LUTTER_IMAGE_HPP
#define LUTTER_IMAGE_HPP

#include <cstdint>
#include <cstddef>
#include <limits>
#include <cassert>
#include <algorithm>
#include "maybe.hpp"

namespace lutter {

template <typename T>
T clamp(T x, T min, T max) { return std::min(std::max(x, min), max); }

template <typename Image>
struct image_encoder_8_bit {
  using pixel_type = typename Image::value_type;
  using channel_type = std::uint8_t;

  // maximum integral value per channel
  static channel_type maxval() {
    return std::numeric_limits<channel_type>::max();
  }

  // encodes the i-th channel (0=red, 1=green, 2=blue)
  static channel_type channel(pixel_type const& p, std::size_t i) {
    auto val = clamp<typename pixel_type::value_type>(p[i], 0, 1);
    return static_cast<channel_type>(val * maxval());
  }
};

// P3 format: http://netpbm.sourceforge.net/doc/ppm.html
template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::ostream& save_p3(std::ostream& os, ImageT const& img) {
  assert(ImageTrait::maxval() < 65536 && "requirement by the p6 format");
  os << "P3\n"
     << "# a raytracing scene\n"
     << img.width() << ' ' << img.height() << '\n'
     << static_cast<int>(ImageTrait::maxval()) << '\n';
  for (std::size_t y=0; y<img.height(); ++y) {
    for (std::size_t x=0; x<img.width(); ++x) {
      for (int channel = 0; channel < 3; ++channel) {
        os << static_cast<int>(ImageTrait::channel(img[y][x], channel))
           << ' ';
      }
      os << '\n';
    }
  }
  return os;
}

} // end namespace lutter

#endif /* LUTTER_IMAGE_HPP */
