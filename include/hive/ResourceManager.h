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
  void registerResourceProcessor(ResourceProcessor<ResourceType>* resourceTypeManager) {
    auto typeId = typeIdFor<ResourceType>();
    m_resourceProcessors[typeId] = resourceTypeManager;
    m_caches[typeId] = new Cache<ResourceType>{this};
  }

  template <typename ResourceType>
  Resource<ResourceType> get(const nu::StringView& name) {
    auto cache = getCacheFor<ResourceType>();
    auto cachedResource = cache->get(name);
    if (cachedResource) {
      return cachedResource;
    }

    // Get a resource processor that will convert an `nu::InputStream` into the requested
    // `ResourceType`.
    auto resourceProcessor = getResourceProcessorFor<ResourceType>();
    if (!resourceProcessor) {
      return Resource<ResourceType>{this};
    }

    // Set up the processor that will do the work of converting the stream into a resource.
    InternalResourceProcessor<ResourceType> processor{this, resourceProcessor};

    // Find the first `ResourceLocator` that will convert the stream into a resource.
    for (auto& i : m_resourceLocators) {
      ResourceLocator* resourceLocator = i.second;

      if (resourceLocator->process(name, &processor)) {
        cache->add(name, processor.result);
        return processor.result;
      }
    }

    // No `ResourceLocator` could convert the stream, so we return an empty resource.
    return Resource<ResourceType>{this, nullptr};
  }

  template <typename ResourceType>
  void add(const nu::StringView& name, const Resource<ResourceType>& resource) {
    auto cache = getCacheFor<ResourceType>();
    cache->add(name, resource);
  }

private:
  DELETE_COPY_AND_MOVE(ResourceManager);

  template <typename ResourceType>
  struct InternalResourceProcessor : ResourceLocator::Processor {
    ResourceManager* resourceManager;
    ResourceProcessor<ResourceType>* resourceProcessor;
    Resource<ResourceType> result;

    InternalResourceProcessor(ResourceManager* resourceManager,
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

  template <typename ResourceType>
  ResourceProcessor<ResourceType>* getResourceProcessorFor() {
    auto typeId = typeIdFor<ResourceType>();
    auto result = m_resourceProcessors.find(typeId);
    if (result == m_resourceProcessors.end()) {
      return nullptr;
    }

    return static_cast<ResourceProcessor<ResourceType>*>(result->second);
  }

  template <typename ResourceType>
  Cache<ResourceType>* getCacheFor() {
    auto typeId = typeIdFor<ResourceType>();
    auto result = m_caches.find(typeId);
    if (result == m_caches.end()) {
      return nullptr;
    }

    return static_cast<Cache<ResourceType>*>(result->second);
  }

  std::map<I32, ResourceLocator*> m_resourceLocators;

  std::unordered_map<MemSize, ResourceProcessorBase*> m_resourceProcessors;
  std::unordered_map<MemSize, CacheBase*> m_caches;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_MANAGER_H_
