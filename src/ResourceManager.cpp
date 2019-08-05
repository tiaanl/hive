#include "hive/ResourceManager.h"

namespace hi {

ResourceManager::ResourceManager() = default;

void ResourceManager::addResourceLocatorFront(ResourceLocator* resourceLocator) {
  auto firstIndex = m_resourceLocators.empty() ? 0 : m_resourceLocators.begin()->first;
  m_resourceLocators[firstIndex - 1] = resourceLocator;
}

void ResourceManager::addResourceLocatorBack(ResourceLocator* resourceLocator) {
  auto lastIndex = m_resourceLocators.empty() ? 0 : m_resourceLocators.rbegin()->first;
  m_resourceLocators[lastIndex + 1] = resourceLocator;
}


}  // namespace hi
