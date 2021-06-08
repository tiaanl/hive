#pragma once

#include "hive/locator.h"
#include "hive/typed_resource.h"

namespace hi {

template <typename ResourceType>
class TypedResourceCache : public TypedResource<ResourceType> {
public:
  explicit TypedResourceCache(nu::ScopedRefPtr<Locator> locator)
    : TypedResource<ResourceType>{std::move(locator)} {}

  ResourceType* get(nu::StringView name) {
    auto resource = TypedResource<ResourceType>::import(name);
    if (!resource) {
      return nullptr;
    }

    auto result = cache_.insert(name, std::move(resource));
    return result.value().get();
  }

private:
  nu::HashMap<nu::DynamicString, nu::ScopedPtr<ResourceType>> cache_;
};

}  // namespace hi
