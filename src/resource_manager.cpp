#include "hive/resource_manager.h"

#include <utility>

namespace hi {

ResourceManager::ResourceManager(nu::ScopedRefPtr<Locator> locator)
  : locator_{std::move(locator)} {}

ResourceManager::~ResourceManager() {
  for (auto& proxy : caches_) {
    proxy.value.deleter(proxy.value.cache);
  }
}

}  // namespace hi
