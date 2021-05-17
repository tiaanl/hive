#pragma once

#include <nucleus/Containers/DynamicArray.h>
#include <nucleus/Memory/ScopedPtr.h>
#include <nucleus/RefCounted.h>
#include <nucleus/Streams/InputStream.h>
#include <nucleus/Text/StaticString.h>
#include <nucleus/Text/StringView.h>

namespace hi {

class Locator : public nu::RefCounted<Locator> {
public:
  virtual ~Locator() = default;

  virtual nu::ScopedPtr<nu::InputStream> locate(nu::StringView name) = 0;
};

}  // namespace hi
