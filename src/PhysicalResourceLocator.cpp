#include "hive/PhysicalResourceLocator.h"

#include "nucleus/Streams/FileInputStream.h"

namespace hi {

namespace {

nu::FilePath buildAbsolutePath(const nu::FilePath& rootPath, const nu::StringView& name) {
  return rootPath / nu::FilePath::normalizeSeparators(name);
}

}  // namespace

PhysicalFileResourceLocator::PhysicalFileResourceLocator(const nu::FilePath& rootPath)
  : m_rootPath{rootPath} {}

bool PhysicalFileResourceLocator::process(const nu::StringView& name, Processor* processor) {
  auto absolutePath = buildAbsolutePath(m_rootPath, name);

  LOG(Info) << "Loading physical file: " << absolutePath.getPath();

  if (!nu::exists(absolutePath)) {
    return false;
  }

  nu::FileInputStream fileInputStream{absolutePath};
  if (!fileInputStream.openedOk()) {
    return false;
  }

  return processor->process(&fileInputStream);
}

}  // namespace hi
