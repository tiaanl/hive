#ifndef HIVE_RESOURCE_CACHE_H_
#define HIVE_RESOURCE_CACHE_H_

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Containers/GrowingArray.h"
#include "nucleus/Macros.h"
#include "nucleus/Memory/ScopedRefPtr.h"
#include "nucleus/Text/StaticString.h"

namespace hi {

template <typename ResourceType>
class ResourceCache {
public:
  ResourceCache() = default;

  void append(const nu::StringView& name, ResourceType&& resource) {

  }

protected:
  struct IndexEntry {
    nu::StaticString<128> name;
    MemSize index;

    //    bool operator==(const nu::StringView& match) {
    //      return name == match;
    //    }
  };

  //  MemSize findIndexEntryWithName(const nu::StringView& name) {
  //    std::find(m_index.begin(), m_index.end(), name);
  //  }

  // nu::DynamicArray<IndexEntry> m_index;
  // nu::GrowingArray<ResourceType>> m_cache;

private:
  DELETE_COPY_AND_MOVE(ResourceCache);
};

}  // namespace hi

#endif  // HIVE_RESOURCE_CACHE_H_
