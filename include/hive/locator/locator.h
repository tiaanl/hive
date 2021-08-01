#pragma once

#include <nucleus/containers/dynamic_array.h>
#include <nucleus/memory/scoped_ptr.h>
#include <nucleus/streams/input_stream.h>
#include <nucleus/text/static_string.h>
#include <nucleus/text/string_view.h>
#include <nucleus/ref_counted.h>

namespace hi {

class Locator : public nu::RefCounted<Locator> {
public:
  virtual ~Locator() = default;

  virtual nu::ScopedPtr<nu::InputStream> locate(nu::StringView name) = 0;
};

}  // namespace hi
