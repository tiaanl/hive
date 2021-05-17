#pragma once

#include "nucleus/Streams/InputStream.h"
#include "nucleus/Text/StringView.h"

namespace hi {

class ResourceManager;

template <typename T>
class Importer {
public:
  virtual ~Importer() = default;

  virtual bool import(nu::StringView name, nu::InputStream* stream, T* storage) = 0;
};

}  // namespace hi
