#pragma once

#include <nucleus/Containers/dynamic_array.h>
#include <nucleus/Memory/scoped_ptr.h>
#include <nucleus/Streams/input_stream.h>
#include <nucleus/Text/static_string.h>
#include <nucleus/Text/string_view.h>
#include <nucleus/ref_counted.h>

namespace hi {

class Locator : public nu::RefCounted<Locator> {
public:
  virtual ~Locator() = default;

  virtual nu::ScopedPtr<nu::InputStream> locate(nu::StringView name) = 0;
};

}  // namespace hi
