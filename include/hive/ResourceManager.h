#ifndef HIVE_RESOURCE_MANAGER_H_
#define HIVE_RESOURCE_MANAGER_H_

#include "hive/Resource.h"
#include "hive/ResourceLocator.h"
#include "hive/ResourceProcessor.h"

#include <map>
#include <unordered_map>

namespace hi {

class ResourceManager {
public:
  ResourceManager() = default;

  void addResourceLocatorFront(ResourceLocator* resourceLocator) {
    auto firstIndex = m_resourceLocators.empty() ? 0 : m_resourceLocators.begin()->first;
    m_resourceLocators[firstIndex - 1] = resourceLocator;
  }

  void addResourceLocatorBack(ResourceLocator* resourceLocator) {
    auto lastIndex = m_resourceLocators.empty() ? 0 : m_resourceLocators.rbegin()->first;
    m_resourceLocators[lastIndex + 1] = resourceLocator;
  }

  template <typename ResourceType>
  void registerResourceType(ResourceProcessor<ResourceType>* resourceTypeManager) {
    auto typeId = ResourceProcessor<ResourceType>::template typeIdFor<ResourceType>();
    m_resourceProcessors[typeId] = resourceTypeManager;
  }

  template <typename ResourceType>
  Resource<ResourceType> get(const nu::StringView& name) {
    nu::InputStream* inputStream = nullptr;

    auto typeId = ResourceProcessor<ResourceType>::template typeIdFor<ResourceType>();
    auto result = m_resourceProcessors.find(typeId);
    if (result == m_resourceProcessors.end()) {
      return Resource<ResourceType>{this};
    }

    auto resourceProcessor = static_cast<ResourceProcessor<ResourceType>*>(result->second);

    DefaultResourceLocatorProcessor<ResourceType> processor{this, resourceProcessor};

    for (auto& i : m_resourceLocators) {
      ResourceLocator* resourceLocator = i.second;

      if (resourceLocator->process(name, &processor)) {
        return processor.result;
      }
    }

    return Resource<ResourceType>{this, nullptr};
  }

private:
  DELETE_COPY_AND_MOVE(ResourceManager);

  template <typename ResourceType>
  struct DefaultResourceLocatorProcessor : ResourceLocator::Processor {
    ResourceManager* resourceManager;
    ResourceProcessor<ResourceType>* resourceProcessor;
    Resource<ResourceType> result;

    DefaultResourceLocatorProcessor(ResourceManager* resourceManager,
                                    ResourceProcessor<ResourceType>* resourceProcessor)
      : resourceManager{resourceManager}, resourceProcessor{resourceProcessor}, result{
                                                                                    resourceManager,
                                                                                    nullptr} {}

    bool process(nu::InputStream* inputStream) override {
      Resource<ResourceType> resource = resourceProcessor->load(resourceManager, inputStream);

      if (!resource) {
        return false;
      }

      result = resource;

      return true;
    }
  };

  std::map<I32, ResourceLocator*> m_resourceLocators;

  std::unordered_map<MemSize, ResourceProcessorBase*> m_resourceProcessors;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_MANAGER_H_
