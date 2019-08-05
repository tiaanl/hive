#ifndef HIVE_RESOURCE_LOCATOR_H_
#define HIVE_RESOURCE_LOCATOR_H_

#include "nucleus/Streams/InputStream.h"
#include "nucleus/Text/StringView.h"

namespace hi {

class ResourceLocator {
public:
  struct Processor {
    virtual ~Processor() = default;
    virtual bool process(nu::InputStream* inputStream) = 0;
  };

  virtual ~ResourceLocator();

  virtual bool process(const nu::StringView& name, Processor* processor) = 0;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_LOCATOR_H_
