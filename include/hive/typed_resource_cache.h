#pragma once

#include "hive/locator/locator.h"
#include "hive/typed_resource.h"

namespace hi {

template <typename ResourceType>
class TypedResourceCache {
public:
  explicit TypedResourceCache(nu::ScopedRefPtr<Locator> locator)
    : typed_resource_{std::move(locator)} {}

  TypedResource<ResourceType>& typed_resource() {
    return typed_resource_;
  }

  Importer<ResourceType>* register_importer(nu::StringView extension,
                                            nu::ScopedPtr<Importer<ResourceType>> importer) {
    return typed_resource_.register_importer(extension, std::move(importer));
  }

  ResourceType* get(nu::StringView name) {
    auto resource = typed_resource_.import(name);
    if (!resource) {
      return nullptr;
    }

    auto result = cache_.insert(name, std::move(resource));
    return result.value().get();
  }

private:
  TypedResource<ResourceType> typed_resource_;
  nu::HashMap<nu::DynamicString, nu::ScopedPtr<ResourceType>> cache_;
};

}  // namespace hi
