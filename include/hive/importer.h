#pragma once

#include "nucleus/Streams/input_stream.h"
#include "nucleus/Text/string_view.h"

namespace hi {

class ResourceManager;

template <typename T>
class Importer {
public:
  virtual ~Importer() = default;

  virtual bool import(nu::StringView name, nu::InputStream* stream, T* storage) = 0;
};

}  // namespace hi
