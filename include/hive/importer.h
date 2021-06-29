#pragma once

#include "nucleus/streams/input_stream.h"
#include "nucleus/text/string_view.h"

namespace hi {

class ResourceManager;

template <typename T>
class Importer {
public:
  virtual ~Importer() = default;

  virtual bool import(nu::StringView name, nu::InputStream* stream, T* storage) = 0;
};

}  // namespace hi
