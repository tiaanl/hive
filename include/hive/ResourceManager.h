#ifndef HIVE_RESOURCE_MANAGER_H_
#define HIVE_RESOURCE_MANAGER_H_

#include "canvas/Renderer/Renderer.h"
#include "canvas/Utils/Image.h"
#include "hive/ResourceCache.h"
#include "hive/TextureResource.h"
#include "nucleus/Containers/GrowingArray.h"
#include "nucleus/FilePath.h"
#include "nucleus/Macros.h"
#include "nucleus/RefCounted.h"

namespace hi {

class ResourceManager {
public:
  explicit ResourceManager(ca::Renderer* renderer);
  ResourceManager(ca::Renderer* renderer, const nu::FilePath& rootPath);

  ca::Image* getImage(const nu::StringView& path);

private:
  DELETE_COPY_AND_MOVE(ResourceManager);

  ca::Renderer* m_renderer;
  nu::FilePath m_rootPath;

  ResourceCache<TextureResource> m_textures;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_MANAGER_H_
