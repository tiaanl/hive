#ifndef HIVE_RESOURCE_MANAGER_H_
#define HIVE_RESOURCE_MANAGER_H_

#include "canvas/Renderer/Renderer.h"
#include "hive/Resource.h"
#include "hive/Resources/File.h"
#include "hive/Resources/Image.h"
#include "hive/Resources/Texture.h"
#include "nucleus/FilePath.h"

namespace hi {

class ResourceManager {
public:
  ResourceManager();
  explicit ResourceManager(const nu::FilePath& rootPath);

  void setRenderer(ca::Renderer* renderer) {
    m_renderer = renderer;
  }

  Resource<File> getFile(const nu::StringView& name);
  Resource<Image> getImage(const nu::StringView& name);
  Resource<Texture> getTexture(const nu::StringView& name);

private:
  DELETE_COPY_AND_MOVE(ResourceManager);

  // The root path used by the resource manager.
  nu::FilePath m_rootPath;

  // Points to a backing renderer.  If it is nullptr, then we don't allow loading of renderer
  // resources.
  ca::Renderer* m_renderer = nullptr;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_MANAGER_H_
