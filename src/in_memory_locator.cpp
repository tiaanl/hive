#include "hive/in_memory_locator.h"

#include <nucleus/Streams/ArrayInputStream.h>

namespace hi {

void InMemoryLocator::set(nu::StringView name, nu::DynamicArray<U8> item) {
  items_.insert(name, std::move(item));
}
nu::ScopedPtr<nu::InputStream> InMemoryLocator::locate(nu::StringView name) {
  auto result = items_.find(name);
  if (!result.was_found()) {
    return {};
  }

  return nu::makeScopedPtr<nu::ArrayInputStream>(result.value().view());
}

}  // namespace hi
