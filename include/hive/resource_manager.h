#pragma once

#include <nucleus/containers/hash_map.h>
#include <nucleus/macros.h>
#include <nucleus/type_id.h>

#include "hive/typed_resource.h"

namespace hi {

class ResourceManager {
  NU_DELETE_COPY_AND_MOVE(ResourceManager);

public:
  explicit ResourceManager(nu::ScopedRefPtr<Locator> locator);

  template <typename ResourceType>
  Importer<ResourceType>* register_importer(nu::StringView extension,
                                            nu::ScopedPtr<Importer<ResourceType>> importer) {
    auto* typed_resource = typed_resource_for<ResourceType>();
    if (!typed_resource) {
      auto insert_result = typed_resources_.insert(nu::type_id_for<ResourceType>(),
                                                   new TypedResource<ResourceType>(locator_));
      DCHECK(insert_result.is_new());
      typed_resource = reinterpret_cast<TypedResource<ResourceType>*>(insert_result.value());
    }

    DCHECK(typed_resource);

    return typed_resource->register_importer(extension, std::move(importer));
  }

  template <typename ResourceType>
  nu::ScopedPtr<ResourceType> import(nu::StringView name) {
    auto* typed_resource = typed_resource_for<ResourceType>();
    if (!typed_resource) {
      LOG(Warning) << "Resource type not supported";
      return {};
    }

    return typed_resource->import(name);
  }

private:
  template <typename ResourceType>
  TypedResource<ResourceType>* typed_resource_for() {
    auto type_id = nu::type_id_for<ResourceType>();

    auto result = typed_resources_.find(type_id);
    if (!result.was_found()) {
      return nullptr;
    }

    return reinterpret_cast<TypedResource<ResourceType>*>(result.value());
  }

  nu::ScopedRefPtr<Locator> locator_;
  nu::HashMap<nu::TypeId, void*> typed_resources_;
};

}  // namespace hi
