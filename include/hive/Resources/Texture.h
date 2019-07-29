#ifndef HIVE_TEXTURE_RESOURCE_H_
#define HIVE_TEXTURE_RESOURCE_H_

#include "canvas/Renderer/Types.h"
#include "canvas/Utils/Size.h"
#include "nucleus/RefCounted.h"

namespace hi {

class Texture : public nu::RefCounted<Texture> {
public:
  Texture(ca::TextureId textureId, const ca::Size& size) : m_textureId{textureId}, m_size{size} {}

  ca::TextureId getTextureId() const {
    return m_textureId;
  }

  const ca::Size& getSize() const {
    return m_size;
  }

private:
  ca::TextureId m_textureId;
  ca::Size m_size;
};

}  // namespace hi

#endif  // HIVE_TEXTURE_RESOURCE_H_
