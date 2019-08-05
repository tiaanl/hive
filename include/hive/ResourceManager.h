#ifndef HIVE_RESOURCE_MANAGER_H_
#define HIVE_RESOURCE_MANAGER_H_

#include "hive/Cache.h"
#include "hive/Converter.h"
#include "hive/ResourceLocator.h"

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
  void registerResourceProcessor(Converter<ResourceType>* resourceTypeManager) {
    auto typeId = typeIdFor<ResourceType>();
    m_typeData.emplace(typeId, new TypeData<ResourceType>{resourceTypeManager});
  }

  template <typename ResourceType>
  ResourceType* get(const nu::StringView& name) {
    auto typeData = getTypeDataFor<ResourceType>();
    if (!typeData) {
      LOG(Error) << "Type not registered";
      return nullptr;
    }

    // If we have it cached already, then we return it.
    auto findResult = typeData->cache.find(name);
    if (findResult.found()) {
      return &findResult.getResource();
    }

    // Set up the processor that will do the work of converting the stream into a resource.
    InternalResourceProcessor<ResourceType> processor{this, typeData->resourceProcessor};

    // Find the first `ResourceLocator` that will convert the stream into a resource.
    for (auto& i : m_resourceLocators) {
      ResourceLocator* resourceLocator = i.second;

      if (resourceLocator->process(name, &processor)) {
        auto insertResult = typeData->cache.insert(name, processor.result);
        if (!insertResult.wasInserted()) {
          LOG(Warning) << "Could not add resource to cache";
          return nullptr;
        }

        return &insertResult.getResource();
      }
    }

    // No `ResourceLocator` could convert the stream, so we return an empty resource.
    return nullptr;
  }

  template <typename ResourceType>
  ResourceType* insert(const nu::StringView& name, ResourceType resource) {
    auto typeData = getTypeDataFor<ResourceType>();
    if (!typeData) {
      LOG(Error) << "Type not registered";
      return nullptr;
    }

    auto result = typeData->cache.insert(name, resource);
    if (!result.wasInserted()) {
      LOG(Error) << "Could not cache resource.";
      return nullptr;
    }

    return &result.getResource();
  }

private:
  DELETE_COPY_AND_MOVE(ResourceManager);

  struct TypeDataBase {
    virtual ~TypeDataBase() = default;
  };

  template <typename ResourceType>
  struct TypeData : public TypeDataBase {
    Converter<ResourceType>* resourceProcessor;
    Cache<ResourceType> cache;

    TypeData(Converter<ResourceType>* resourceProcessor) : resourceProcessor{resourceProcessor} {}
  };

  template <typename ResourceType>
  struct InternalResourceProcessor : ResourceLocator::Processor {
    ResourceManager* resourceManager;
    Converter<ResourceType>* resourceProcessor;
    ResourceType result;

    InternalResourceProcessor(ResourceManager* resourceManager,
                              Converter<ResourceType>* resourceProcessor)
      : resourceManager{resourceManager}, resourceProcessor{resourceProcessor} {}

    bool process(nu::InputStream* inputStream) override {
      return resourceProcessor->load(resourceManager, inputStream, &result);
    }
  };

  template <typename ResourceType>
  TypeData<ResourceType>* getTypeDataFor() {
    auto typeId = typeIdFor<ResourceType>();
    auto it = m_typeData.find(typeId);
    if (it == m_typeData.end()) {
      return nullptr;
    }

    return static_cast<TypeData<ResourceType>*>(it->second);
  }

  std::map<I32, ResourceLocator*> m_resourceLocators;
  std::unordered_map<TypeId, TypeDataBase*> m_typeData;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_MANAGER_H_
