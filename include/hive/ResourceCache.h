#ifndef HIVE_RESOURCE_CACHE_H_
#define HIVE_RESOURCE_CACHE_H_

#include "hive/Resource.h"
#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Containers/GrowingArray.h"
#include "nucleus/Logging.h"
#include "nucleus/Macros.h"
#include "nucleus/Memory/ScopedRefPtr.h"
#include "nucleus/Text/StaticString.h"

namespace hi {

class ResourceManager;

template <typename ResourceType>
class ResourceCache {
public:
  ResourceCache() = default;

  Resource<ResourceType> find(const nu::StringView& name) {
    for (Entry& entry : m_entries) {
      if (entry.name == name) {
        return entry.resource;
      }
    }

    return Resource<ResourceType>{nullptr};
  }

  Resource<ResourceType> append(const nu::StringView& name, const Resource<ResourceType>& resource) {
    Entry* found = nullptr;
    for (MemSize i = 0; i < m_entries.getSize(); ++i) {
      Entry* entry = &m_entries[i];
      if (entry->name == name) {
        // LOG(Error) << "Resource is already cached! (" << name << ")";
        return entry->resource;
      }
    }

    m_entries.emplaceBack(name, std::move(resource));

    return m_entries.last().resource;
  }

protected:
  struct Entry {
    nu::StaticString<128> name;
    Resource<ResourceType> resource;
  };

  nu::DynamicArray<Entry> m_entries;

private:
  DELETE_COPY_AND_MOVE(ResourceCache);
};

}  // namespace hi

#endif  // HIVE_RESOURCE_CACHE_H_
