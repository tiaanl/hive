#pragma once

#include <nucleus/Containers/DynamicArray.h>
#include <nucleus/Containers/hash_map.h>
#include <nucleus/Memory/ScopedPtr.h>
#include <nucleus/Text/DynamicString.h>

#include "hive/locator.h"

namespace hi {

class InMemoryLocator : public Locator {
public:
  void set(nu::StringView name, nu::DynamicArray<U8> item);

  // Override: Locator
  nu::ScopedPtr<nu::InputStream> locate(nu::StringView name) override;

private:
  nu::HashMap<nu::DynamicString, nu::DynamicArray<U8>> items_;
};

}  // namespace hi
