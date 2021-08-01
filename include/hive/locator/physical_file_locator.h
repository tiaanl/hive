#pragma once

#include <nucleus/file_path.h>

#include "locator.h"

namespace hi {

class PhysicalFileLocator : public Locator {
  NU_DELETE_COPY_AND_MOVE(PhysicalFileLocator);

public:
  PhysicalFileLocator();
  explicit PhysicalFileLocator(nu::FilePath rootPath);

  const nu::FilePath& root_path() const {
    return m_rootPath;
  }

  void root_path(const nu::FilePath&);

  // Override: Locator
  nu::ScopedPtr<nu::InputStream> locate(nu::StringView name) override;

private:
  nu::FilePath m_rootPath;
};

}  // namespace hi
