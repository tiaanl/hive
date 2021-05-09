#pragma once

#include "hive/Converter.h"
#include "hive/ResourceLocator.h"
#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Containers/hash_map.h"
#include "nucleus/Memory/ScopedPtr.h"
#include "nucleus/Text/DynamicString.h"

namespace hi {

class ResourceManager {
  NU_DELETE_COPY_AND_MOVE(ResourceManager);

public:
  ResourceManager();

  void add_resource_locator(ResourceLocator* resourceLocator);

  template <typename ResourceType>
  void registerConverter(Converter<ResourceType>* resourceTypeManager) {
    auto typeId = typeIdFor<ResourceType>();
    m_typeData.insert({typeId, new TypeData<ResourceType>{resourceTypeManager}});
  }

  template <typename ResourceType>
  ResourceType* get(const nu::StringView& name) {
    auto* typeData = getTypeDataFor<ResourceType>();
    if (!typeData) {
      LOG(Error) << "Type not registered.";
      return nullptr;
    }

    // If we have it cached already, then we return it.
    auto findResult = typeData->cache.find(name);
    if (findResult.was_found()) {
      return &findResult.value();
    }

    // Set up the processor that will do the work of converting the stream into a resource.
    InternalResourceProcessor<ResourceType> processor{this, typeData->resourceProcessor};

    // Find the first `ResourceLocator` that will convert the stream into a resource.
    for (auto* resource_locator : m_resourceLocators) {
      if (resource_locator->process(name, &processor)) {
        auto [it, inserted] = typeData->cache.insert({name, std::move(processor.result)});
        return &it->second;
      }
    }

    // No `ResourceLocator` could convert the stream, so we return an empty resource.
    return nullptr;
  }

  template <typename ResourceType>
  ResourceType* insert(const nu::StringView& name, ResourceType resource) {
    auto typeData = getTypeDataFor<ResourceType>();
    if (!typeData) {
      LOG(Error) << "Type not registered.";
      return nullptr;
    }

    auto result = typeData->cache.insert(name, resource);
    return &result.value();
  }

  template <typename ResourceType>
  void release(ResourceType* NU_UNUSED(resourceType)) {
    auto* typeData = getTypeDataFor<ResourceType>();
    if (!typeData) {
      LOG(Error) << "Type not registered.";
      return;
    }

    // TODO: Release the resource :)
  }

private:
  template <typename T>
  static MemSize typeIdFor() {
    static const char typeIdType = '\0';
    return reinterpret_cast<MemSize>(&typeIdType);
  }

  struct TypeDataBase {
    virtual ~TypeDataBase() = default;
  };

  template <typename ResourceType>
  struct TypeData : public TypeDataBase {
    Converter<ResourceType>* resourceProcessor;
    nu::HashMap<nu::DynamicString, ResourceType> cache;

    explicit TypeData(Converter<ResourceType>* resourceProcessor)
      : resourceProcessor{resourceProcessor} {}
  };

  template <typename ResourceType>
  struct InternalResourceProcessor : ResourceLocator::Processor {
    ResourceManager* resourceManager;
    Converter<ResourceType>* converter;
    ResourceType result;

    InternalResourceProcessor(ResourceManager* resourceManager, Converter<ResourceType>* converter)
      : resourceManager{resourceManager}, converter{converter} {}

    bool process(nu::StringView name, nu::InputStream* inputStream) override {
      return converter->load(resourceManager, name, inputStream, &result);
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

  std::vector<ResourceLocator*> m_resourceLocators;
  std::unordered_map<MemSize, TypeDataBase*> m_typeData;
};

}  // namespace hi
