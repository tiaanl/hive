#pragma once

#include <nucleus/containers/hash_map.h>
#include <nucleus/macros.h>
#include <nucleus/type_id.h>

#include "hive/typed_resource_cache.h"

namespace hi {

class ResourceManager : public nu::RefCounted<ResourceManager> {
  NU_DELETE_COPY_AND_MOVE(ResourceManager);

public:
  explicit ResourceManager(nu::ScopedRefPtr<Locator> locator);

  ~ResourceManager();

  template <typename ResourceType>
  Importer<ResourceType>* register_importer(nu::StringView extension,
                                            nu::ScopedPtr<Importer<ResourceType>> importer) {
    auto* typed_resource = typed_resource_cache_for<ResourceType>();
    if (!typed_resource) {
      CacheProxy proxy = {
          new TypedResourceCache<ResourceType>(locator_),
          reinterpret_cast<CacheProxy::DeleterFunc>(&deleter<ResourceType>),
      };

      auto insert_result = caches_.insert(nu::type_id_for<ResourceType>(), proxy);
      DCHECK(insert_result.is_new());
      typed_resource =
          reinterpret_cast<TypedResourceCache<ResourceType>*>(insert_result.value().cache);
    }

    DCHECK(typed_resource);

    return typed_resource->register_importer(extension, std::move(importer));
  }

  template <typename ResourceType>
  nu::ScopedPtr<ResourceType> import(nu::StringView name) {
    auto* typed_resource = typed_resource_cache_for<ResourceType>();
    if (!typed_resource) {
      LOG(Warning) << "Resource type not supported";
      return {};
    }

    return typed_resource->typed_resource().import(name);
  }

  template <typename ResourceType>
  ResourceType* get(nu::StringView name) {
    auto* typed_resource = typed_resource_cache_for<ResourceType>();
    if (!typed_resource) {
      LOG(Warning) << "Resource type not supported";
      return {};
    }

    return typed_resource->get(name);
  }

private:
  template <typename ResourceType>
  TypedResourceCache<ResourceType>* typed_resource_cache_for() {
    auto type_id = nu::type_id_for<ResourceType>();

    auto result = caches_.find(type_id);
    if (!result.was_found()) {
      return nullptr;
    }

    return reinterpret_cast<TypedResourceCache<ResourceType>*>(result.value().cache);
  }

  template <typename ResourceType>
  static void deleter(void* ptr) {
    delete static_cast<TypedResourceCache<ResourceType>*>(ptr);
  }

  struct CacheProxy {
    using DeleterFunc = void (*)(void*);

    void* cache;
    DeleterFunc deleter;
  };

  nu::ScopedRefPtr<Locator> locator_;
  nu::HashMap<nu::TypeId, CacheProxy> caches_;
};

}  // namespace hi
