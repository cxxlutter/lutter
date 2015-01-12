#include <iostream>
#include <LunchTimeTracer/Image.hpp>
#include "image.hpp"

namespace lutter {

std::ostream& save_p3(std::ostream& os, erased_image<std::uint8_t>& img) {
  // P3 format: http://netpbm.sourceforge.net/doc/ppm.html
  os << "P3\n"
     << "# a raytracing scene\n"
     << img.width() << ' ' << img.height() << '\n'
     << static_cast<int>(std::numeric_limits<uint8_t>::max()) << '\n';
  for (std::size_t y=0; y<img.height(); ++y) {
    for (std::size_t x=0; x<img.width(); ++x) {
      for (int channel = 0; channel < 3; ++channel) {
        os << static_cast<int>(img.at(y, x, channel)) << ' ';
      }
      os << '\n';
    }
  }
  return os;
}

std::ostream& save_bmp(std::ostream& os, erased_image<std::uint8_t>& img) {
  LTT_TrippyColors::Image trippy_image(img.width(), img.height());
  std::vector<std::uint8_t> channel_buf(3*img.width());
  for (std::size_t y=0, y_end=img.height(), x_end=img.width(); y<y_end; ++y) {
    img.copy(y, y+1, 0, x_end, 0, 3, &*channel_buf.begin());
    auto it = channel_buf.begin();
    for (std::size_t x=0; x<x_end; ++x) {
      auto r = *it++;
      auto g = *it++;
      auto b = *it++;
      trippy_image(x, y) = LTT_TrippyColors::ImageColor(r, g, b);
    }
  }
  LTT_TrippyColors::ToBmp(trippy_image, os);
  return os;
}

std::ostream& save_image(std::ostream& os, erased_image<std::uint8_t>& img,
                         image_type format) {
  switch (format) {
  case image_type::p3: return save_p3(os, img);
  case image_type::bmp: return save_bmp(os, img);
  default: assert(false && "wrong image_type");
  }
  assert(false);
}

maybe<image_type> image_type_from_string(string_ref type) {
  std::pair<const char*, image_type> types[] = {
    { "p3", lutter::image_type::p3 },
    { "bmp", lutter::image_type::bmp },
  };
  for (auto& p : types)
    if (type == p.first)
      return p.second;
  return nothing();
}

} // end namespace lutter
