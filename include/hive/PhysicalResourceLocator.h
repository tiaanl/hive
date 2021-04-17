#pragma once

#include "hive/ResourceLocator.h"
#include "nucleus/FilePath.h"

namespace hi {

class PhysicalFileResourceLocator : public hi::ResourceLocator {
public:
  NU_DELETE_COPY_AND_MOVE(PhysicalFileResourceLocator);

  PhysicalFileResourceLocator();
  explicit PhysicalFileResourceLocator(const nu::FilePath& rootPath);

  const nu::FilePath& getRootPath() const {
    return m_rootPath;
  }

  void setRootPath(const nu::FilePath&);

  // Override: hi::ResourceLocator
  bool process(nu::StringView name, Processor* processor) override;

private:
  nu::FilePath m_rootPath;
};

}  // namespace hi
