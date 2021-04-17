#pragma once

#include "nucleus/Streams/InputStream.h"
#include "nucleus/Text/StringView.h"

namespace hi {

class ResourceLocator {
public:
  struct Processor {
    virtual ~Processor() = default;
    virtual bool process(nu::StringView name, nu::InputStream* inputStream) = 0;
  };

  virtual ~ResourceLocator();

  virtual bool process(nu::StringView name, Processor* processor) = 0;
};

}  // namespace hi
