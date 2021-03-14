#include "hive/PhysicalResourceLocator.h"

#include "nucleus/Streams/FileInputStream.h"

namespace hi {

namespace {

nu::FilePath buildAbsolutePath(const nu::FilePath& rootPath, const nu::StringView& name) {
  return rootPath / nu::FilePath::normalizeSeparators(name);
}

}  // namespace

PhysicalFileResourceLocator::PhysicalFileResourceLocator() = default;

PhysicalFileResourceLocator::PhysicalFileResourceLocator(const nu::FilePath& rootPath)
  : m_rootPath{rootPath} {}

void PhysicalFileResourceLocator::setRootPath(const nu::FilePath& rootPath) {
  m_rootPath = rootPath;
}

bool PhysicalFileResourceLocator::process(nu::StringView name, Processor* processor) {
  auto absolutePath = buildAbsolutePath(m_rootPath, name);

  if (!nu::exists(absolutePath)) {
    LOG(Warning) << "Physical file not found: " << absolutePath.getPath();
    return false;
  }

  LOG(Info) << "Loading physical file: " << absolutePath.getPath();

  nu::FileInputStream fileInputStream{absolutePath};
  if (!fileInputStream.openedOk()) {
    LOG(Error) << "Could not open physical file: " << absolutePath.getPath();
    return false;
  }

  return processor->process(name, &fileInputStream);
}

}  // namespace hi
