#pragma once

#include <nucleus/Containers/hash_map.h>
#include <nucleus/Containers/stable_pool.h>
#include <nucleus/Memory/ScopedPtr.h>
#include <nucleus/Memory/ScopedRefPtr.h>
#include <nucleus/Text/DynamicString.h>

#include "hive/importer.h"
#include "locator.h"

namespace hi {

template <typename ResourceType>
class TypedResource {
public:
  explicit TypedResource(nu::ScopedRefPtr<Locator> locator) : locator_{std::move(locator)} {}

  Importer<ResourceType>* register_importer(nu::StringView extension,
                                            nu::ScopedPtr<Importer<ResourceType>> importer) {
    auto result = importers_.insert(extension, std::move(importer));
    return result.value().get();
  }

  Importer<ResourceType>* importer_for_extension(nu::StringView extension) {
    auto result = importers_.find(extension);
    if (!result.was_found()) {
      return nullptr;
    }

    return result.value().get();
  }

  ResourceType* import(nu::StringView name) {
    auto extension = get_extension_from_name(name);
    if (extension.empty()) {
      LOG(Warning) << "Resource name does not have an extension: " << name;
      return {};
    }

    auto* importer = importer_for_extension(extension);
    if (!importer) {
      LOG(Warning) << "No importer for extension: " << extension;
      return {};
    }

    auto stream = locator_->locate(name);
    if (!stream) {
      LOG(Warning) << "Resource not found: " << name;
      return {};
    }

    // TODO: This means resources should be default constructed, which is not so nice.
    auto resource = nu::makeScopedPtr<ResourceType>();

    if (!importer->import(name, stream.get(), resource.get())) {
      LOG(Warning) << "Importer failed for: " << name;
      return {};
    }

    return cache_.construct(std::move(*resource.release()));
  }

  bool remove(ResourceType* resource) {
    return cache_.remove(resource);
  }

private:
  nu::StringView get_extension_from_name(nu::StringView name) {
    auto it = name.find_last_of('.');
    if (it == nu::StringView::npos) {
      return {};
    }

    return name.subString(it + 1);
  }

  nu::ScopedRefPtr<Locator> locator_;
  nu::HashMap<nu::DynamicString, nu::ScopedPtr<Importer<ResourceType>>> importers_;
  nu::StablePool<ResourceType> cache_;
};

}  // namespace hi
