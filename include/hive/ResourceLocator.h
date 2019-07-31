#ifndef HIVE_RESOURCE_LOCATOR_H_
#define HIVE_RESOURCE_LOCATOR_H_

#include "nucleus/FilePath.h"
#include "nucleus/Macros.h"
#include "nucleus/Streams/FileInputStream.h"
#include "nucleus/Streams/InputStream.h"
#include "nucleus/Text/StringView.h"

namespace hi {

class ResourceLocator {
public:
  struct Processor {
    virtual ~Processor() = default;
    virtual bool process(nu::InputStream* inputStream) = 0;
  };

  virtual ~ResourceLocator() = default;

  virtual bool process(const nu::StringView& name, Processor* processor) = 0;
};

class PhysicalFileResourceLocator : public ResourceLocator {
public:
  explicit PhysicalFileResourceLocator(const nu::FilePath& rootPath) : m_rootPath{rootPath} {}

  bool process(const nu::StringView& name, Processor* processor) override {
    auto absolutePath = buildAbsolutePath(name);

    if (!nu::exists(absolutePath)) {
      return false;
    }

    nu::FileInputStream fileInputStream{absolutePath};
    if (!fileInputStream.openedOk()) {
      return false;
    }

    return processor->process(&fileInputStream);
  }

private:
  DELETE_COPY_AND_MOVE(PhysicalFileResourceLocator);

  nu::FilePath buildAbsolutePath(const nu::StringView& name) {
    return m_rootPath / nu::FilePath::normalizeSeparators(name);
  }

  nu::FilePath m_rootPath;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_LOCATOR_H_
