#include "hive/locator/physical_file_locator.h"

#include "nucleus/streams/file_input_stream.h"

namespace hi {

namespace {

nu::FilePath build_absolute_path(const nu::FilePath& rootPath, const nu::StringView& name) {
  return rootPath / nu::FilePath::normalizeSeparators(name);
}

}  // namespace

PhysicalFileLocator::PhysicalFileLocator()
  : PhysicalFileLocator{nu::getCurrentWorkingDirectory()} {}

PhysicalFileLocator::PhysicalFileLocator(nu::FilePath root_path)
  : root_path_{std::move(root_path)} {}

void PhysicalFileLocator::root_path(const nu::FilePath& root_path) {
  root_path_ = root_path;
}
nu::ScopedPtr<nu::InputStream> PhysicalFileLocator::locate(nu::StringView name) {
  auto absolute_path = build_absolute_path(root_path_, name);

  if (!nu::exists(absolute_path)) {
    // LOG(Warning) << "Physical file not found: " << absolute_path.getPath();
    return {};
  }

  // LOG(Info) << "Loading physical file: " << absolutePath.getPath();

  auto stream = nu::make_scoped_ptr<nu::FileInputStream>(absolute_path);
  if (!stream->openedOk()) {
    return {};
  }

  return stream;
}

}  // namespace hi
