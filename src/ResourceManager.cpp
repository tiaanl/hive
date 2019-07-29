#include "hive/ResourceManager.h"

#include "nucleus/Logging.h"
#include "nucleus/Streams/FileInputStream.h"

namespace hi {

namespace {

nu::FilePath buildFilePath(const nu::FilePath& root, const nu::StringView& path) {
  return root.append(path);
}

}  // namespace

ResourceManager::ResourceManager(ca::Renderer* renderer)
  : m_renderer{renderer}, m_rootPath{nu::getCurrentWorkingDirectory()} {}

ResourceManager::ResourceManager(ca::Renderer* renderer, const nu::FilePath& rootPath)
  : m_renderer{renderer}, m_rootPath{rootPath} {}

ca::Image* ResourceManager::getImage(const nu::StringView& path) {
  auto fullPath = buildFilePath(m_rootPath, path);

  nu::FileInputStream stream{fullPath};
  if (!stream.openedOk()) {
    LOG(Error) << "Could not open resource file. (" << fullPath << ")";
    return nullptr;
  }

  ca::Image result;
  if (!result.loadFromStream(&stream)) {
    return nullptr;
  }

  return nullptr;
}

}  // namespace hi
