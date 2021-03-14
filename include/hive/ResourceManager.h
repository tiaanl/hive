#pragma once

#include <map>

#include "hive/Converter.h"
#include "hive/ResourceLocator.h"
#include "nucleus/Containers/hash_map.h"
#include "nucleus/Memory/ScopedPtr.h"
#include "nucleus/Text/DynamicString.h"

namespace hi {

class ResourceManager {
public:
  NU_DELETE_COPY_AND_MOVE(ResourceManager);

  ResourceManager();

  void addResourceLocatorFront(ResourceLocator* resourceLocator);
  void addResourceLocatorBack(ResourceLocator* resourceLocator);

  template <typename ResourceType>
  void registerConverter(Converter<ResourceType>* resourceTypeManager) {
    auto typeId = typeIdFor<ResourceType>();
    nu::ScopedPtr<TypeDataBase> typeData{new TypeData<ResourceType>{resourceTypeManager}};
    m_typeData.set(typeId, std::move(typeData));
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
    if (findResult != typeData->cache.end()) {
      return &findResult->value;
    }

    // Set up the processor that will do the work of converting the stream into a resource.
    InternalResourceProcessor<ResourceType> processor{this, typeData->resourceProcessor};

    // Find the first `ResourceLocator` that will convert the stream into a resource.
    for (auto& i : m_resourceLocators) {
      ResourceLocator* resourceLocator = i.second;

      if (resourceLocator->process(name, &processor)) {
        auto result = typeData->cache.set(name, std::move(processor.result));
        return &result.value();
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

    auto result = typeData->cache.set(name, resource);
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

    return static_cast<TypeData<ResourceType>*>(it->value.get());
  }

  std::map<I32, ResourceLocator*> m_resourceLocators;
  nu::HashMap<MemSize, nu::ScopedPtr<TypeDataBase>> m_typeData;
};

}  // namespace hi
