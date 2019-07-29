#ifndef HIVE_TEXTURE_CACHE_H_
#define HIVE_TEXTURE_CACHE_H_

#include "hive/ResourceCache.h"
#include "hive/resources/Texture.h"

namespace hi {

class TextureCache : ResourceCache<Texture> {
public:
  TextureCache();

  void create(const nu::StringView& name, ca::TextureId textureId, const ca::Size& size);

private:
  DELETE_COPY_AND_MOVE(TextureCache);
};

}  // namespace hi

#endif  // HIVE_TEXTURE_CACHE_H_
