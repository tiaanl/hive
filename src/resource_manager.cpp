#include "hive/resource_manager.h"

#include <utility>

namespace hi {

ResourceManager::ResourceManager(nu::ScopedRefPtr<Locator> locator)
  : locator_{std::move(locator)} {}

}  // namespace hi
