#ifndef HIVE_PHYSICAL_RESOURCE_LOCATOR_H_
#define HIVE_PHYSICAL_RESOURCE_LOCATOR_H_

#include "hive/ResourceLocator.h"
#include "nucleus/FilePath.h"

namespace hi {

class PhysicalFileResourceLocator : public hi::ResourceLocator {
public:
  explicit PhysicalFileResourceLocator(const nu::FilePath& rootPath);

  bool process(const nu::StringView& name, Processor* processor) override;

private:
  DELETE_COPY_AND_MOVE(PhysicalFileResourceLocator);

  nu::FilePath m_rootPath;
};

}  // namespace hi

#endif  // HIVE_PHYSICAL_RESOURCE_LOCATOR_H_
