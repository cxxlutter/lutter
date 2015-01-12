#ifndef LUTTER_IMAGE_HPP
#define LUTTER_IMAGE_HPP

#include <cstdint>
#include <cstddef>
#include <limits>
#include <cassert>
#include <algorithm>
#include <iosfwd>
#include <cassert>
#include <memory>
#include "maybe.hpp"
#include "string_ref.hpp"

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

template <typename ChannelT = std::uint8_t>
struct erased_image {
  virtual ~erased_image() =default;
  virtual std::size_t height() const =0;
  virtual std::size_t width() const =0;
  virtual ChannelT at(std::size_t y, std::size_t x, std::size_t channel) const =0;
  virtual void copy(std::size_t y_start, std::size_t y_end,
                    std::size_t x_start, std::size_t x_end,
                    std::size_t channel_start, std::size_t channel_end,
                    ChannelT *out) const {
    assert(y_start < y_end);
    assert(y_end <= height());
    assert(x_start < x_end);
    assert(x_end <= width());
    assert(channel_start < channel_end);
    for (std::size_t y=y_start; y<y_end; ++y)
      for (std::size_t x=x_start; x<x_end; ++x)
        for (std::size_t c=channel_start; c<channel_end; ++c)
          *out++ = at(y, x, c);
  }
};
template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::unique_ptr<erased_image<typename ImageTrait::channel_type> > erase_image(ImageT& img) {
  using channel_type = typename ImageTrait::channel_type;
  struct eraser : erased_image<channel_type> {
    eraser(ImageT *img) : img(img) {}
    ImageT *img;
    std::size_t height() const override { return img->height(); }
    std::size_t width() const override { return img->width(); }
    channel_type at(std::size_t y, std::size_t x, std::size_t channel) const override {
      return ImageTrait::channel((*img)[y][x], channel);
    }
    void copy(std::size_t y_start, std::size_t y_end,
              std::size_t x_start, std::size_t x_end,
              std::size_t channel_start, std::size_t channel_end,
              channel_type *out) const override {
      assert(y_start < y_end);
      assert(y_end <= height());
      assert(x_start < x_end);
      assert(x_end <= width());
      assert(channel_start < channel_end);
      for (std::size_t y=y_start; y<y_end; ++y)
        for (std::size_t x=x_start; x<x_end; ++x)
          for (std::size_t c=channel_start; c<channel_end; ++c)
            *out++ = ImageTrait::channel((*img)[y][x], c);
    }
  };
  return std::unique_ptr<erased_image<channel_type> >(new eraser(&img));
}

std::ostream& save_p3(std::ostream& os, erased_image<std::uint8_t>& img);

template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::ostream& save_p3(std::ostream& os, ImageT img) {
  auto erased = erase_image<ImageT, ImageTrait>(img);
  return save_p3(os, *erased);
}

std::ostream& save_bmp(std::ostream& os, erased_image<std::uint8_t>& img);

template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::ostream& save_bmp(std::ostream& os, ImageT img) {
  auto erased = erase_image<ImageT, ImageTrait>(img);
  return save_bmp(os, *erased);
}

enum class image_type {
  p3,
  bmp,
};
maybe<image_type> image_type_from_string(string_ref type);

std::ostream& save_image(std::ostream& os, erased_image<std::uint8_t>& img,
                         image_type format);

template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::ostream& save_image(std::ostream& os, ImageT img, image_type format) {
  auto erased = erase_image<ImageT, ImageTrait>(img);
  return save_image(os, *erased, format);
}

} // end namespace lutter

#endif /* LUTTER_IMAGE_HPP */
