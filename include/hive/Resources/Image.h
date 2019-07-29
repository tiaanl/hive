#ifndef HIVE_IMAGE_H_
#define HIVE_IMAGE_H_

#include "nucleus/RefCounted.h"
#include "canvas/Utils/Image.h"

#include <type_traits>

namespace hi {

class Image : public nu::RefCounted<Image> {
public:
  Image(ca::Image&& image) : m_image{std::move(image)} {}

  const ca::Image& image() const {
    return m_image;
  }

private:
  DELETE_COPY_AND_MOVE(Image);

  ca::Image m_image;
};

}  // namespace hi

#endif  // HIVE_TEXTURE_RESOURCE_H_
