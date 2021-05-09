#include "hive/ResourceManager.h"

namespace hi {

ResourceManager::ResourceManager() = default;

void ResourceManager::add_resource_locator(ResourceLocator* resourceLocator) {
  m_resourceLocators.emplace_back(resourceLocator);
}

}  // namespace hi
