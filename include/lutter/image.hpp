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
#include "utils.hpp"

namespace lutter {

template <typename Image>
struct image_encoder_8_bit {
  using pixel_type = typename Image::value_type;
  using channel_type = std::uint8_t;

  // maximum integral value per channel
  static channel_type maxval();

  // encodes the i-th channel (0=red, 1=green, 2=blue)
  static channel_type channel(pixel_type const& p, std::size_t i);
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
                    ChannelT *out) const;
};
template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::unique_ptr<erased_image<typename ImageTrait::channel_type> >
erase_image(ImageT& img);

std::ostream& save_p3(std::ostream& os, erased_image<std::uint8_t>& img);

template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::ostream& save_p3(std::ostream& os, ImageT img);

std::ostream& save_bmp(std::ostream& os, erased_image<std::uint8_t>& img);

template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::ostream& save_bmp(std::ostream& os, ImageT img);

enum class image_type {
  p3,
  bmp,
};
maybe<image_type> image_type_from_string(string_ref type);

std::ostream& save_image(std::ostream& os, erased_image<std::uint8_t>& img,
                         image_type format);

template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::ostream& save_image(std::ostream& os, ImageT img, image_type format);

} // end namespace lutter

#include "bits/image.hpp"

#endif /* LUTTER_IMAGE_HPP */
