namespace lutter {

template <typename Image>
auto image_encoder_8_bit<Image>::maxval() -> channel_type {
  return std::numeric_limits<channel_type>::max();
}

template <typename Image>
auto image_encoder_8_bit<Image>::channel(pixel_type const& p,
                                         std::size_t i) -> channel_type {
  auto val = clamp<typename pixel_type::value_type>(p[i], 0, 1);
  return static_cast<channel_type>(val * maxval());
}

namespace detail {

template <typename ChannelT = std::uint8_t, typename AtFunc>
void copy_impl(std::size_t y_start, std::size_t y_end,
               std::size_t x_start, std::size_t x_end,
               std::size_t channel_start, std::size_t channel_end,
               ChannelT *out, AtFunc at)
{
  assert(y_start < y_end);
  assert(x_start < x_end);
  assert(channel_start < channel_end);
  for (std::size_t y=y_start; y<y_end; ++y)
    for (std::size_t x=x_start; x<x_end; ++x)
      for (std::size_t c=channel_start; c<channel_end; ++c)
        *out++ = at(y, x, c);
}

} // end namespace detail

template <typename ChannelT>
void erased_image<ChannelT>::copy(std::size_t y_start, std::size_t y_end,
                                  std::size_t x_start, std::size_t x_end,
                                  std::size_t c_start, std::size_t c_end,
                                  ChannelT *out) const {
  assert(y_end <= height());
  assert(x_end <= width());
  detail::copy_impl(y_start, y_end, x_start, x_end, c_start, c_end, out,
                    [this](std::size_t y, std::size_t x, std::size_t c) {
                      return this->at(y, x, c);
                    });
}

template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::unique_ptr<erased_image<typename ImageTrait::channel_type> >
erase_image(ImageT& img) {
  using channel_t = typename ImageTrait::channel_type;

  struct eraser : erased_image<channel_t> {
    eraser(ImageT *img) : img(img) {}
    ImageT *img;
    std::size_t height() const override { return img->height(); }
    std::size_t width() const override { return img->width(); }
    channel_t at(std::size_t y, std::size_t x,
                 std::size_t channel) const override {
      return ImageTrait::channel((*img)[y][x], channel);
    }
    void copy(std::size_t y0, std::size_t y1, std::size_t x0, std::size_t x1,
              std::size_t c0, std::size_t c1, channel_t *out) const override {
      assert(y1 <= height());
      assert(x1 <= width());
      ImageT *img_ptr = img;
      return detail::copy_impl(y0, y1, x0, x1, c0, c1, out,
                               [img_ptr](std::size_t y, std::size_t x,
                                     std::size_t c) {
                                 return ImageTrait::channel((*img_ptr)[y][x], c);
                               });
    }
  };
  return std::unique_ptr<erased_image<channel_t> >(new eraser(&img));
}

template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::ostream& save_p3(std::ostream& os, ImageT img) {
  return save_p3(os, *erase_image<ImageT, ImageTrait>(img));
}

template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::ostream& save_bmp(std::ostream& os, ImageT img) {
  return save_bmp(os, *erase_image<ImageT, ImageTrait>(img));
}

template <typename ImageT, typename ImageTrait = image_encoder_8_bit<ImageT> >
std::ostream& save_image(std::ostream& os, ImageT img, image_type format) {
  return save_image(os, *erase_image<ImageT, ImageTrait>(img), format);
}

} // end namespace lutter
