#include "hive/ResourceManager.h"

#include "nucleus/Logging.h"

namespace hi {

namespace {

nu::FilePath buildFilePath(const nu::FilePath& root, const nu::StringView& path) {
  return root.append(path);
}

struct ImageFileProcessor : public FileProcessor {
  ca::Image image;

  void process(nu::InputStream* stream) override {
    image.loadFromStream(stream);
  }
};

}  // namespace

ResourceManager::ResourceManager() : m_rootPath{nu::getCurrentWorkingDirectory()} {}

ResourceManager::ResourceManager(const nu::FilePath& rootPath) : m_rootPath{rootPath} {}

Resource<File> ResourceManager::getFile(const nu::StringView& name) {
  auto filePath = buildFilePath(m_rootPath, name);

  Resource<File> result{this, new PhysicalFile{filePath}};

  return result;
}

Resource<Image> ResourceManager::getImage(const nu::StringView& name) {
  auto file = getFile(name);
  ImageFileProcessor processor;
  file->process(&processor);
  return {this, new Image{std::move(processor.image)}};
}

Resource<Texture> ResourceManager::getTexture(const nu::StringView& name) {
  auto image = getImage(name);

  if (!m_renderer) {
    LOG(Warning) << "Creating texture without renderer.";
    return {this, nullptr};
  }

  ca::TextureId textureId = m_renderer->createTexture(image->image());
  return {this, new Texture{textureId, image->image().getSize()}};
}

}  // namespace hi
