#ifndef HIVE_CACHE_H_
#define HIVE_CACHE_H_

#include "nucleus/Containers/GrowingArray.h"
#include "nucleus/Text/StaticString.h"

namespace hi {

template <typename ResourceType>
class Cache {
public:
  Cache() = default;

  class FindResult {
  public:
    friend class Cache;

    bool found() const {
      return m_found;
    }

    const nu::StringView& getName() const {
      return *m_name;
    }

    ResourceType& getResource() const {
      return *m_resource;
    }

  private:
    FindResult(bool found, nu::StringView* name, ResourceType* resource)
      : m_found{found}, m_name{name}, m_resource{resource} {}

    bool m_found;
    nu::StringView* m_name = nullptr;
    ResourceType* m_resource;
  };

  auto find(const nu::StringView& name) -> FindResult {
    for (Entry& entry : m_entries) {
      if (entry.name == name) {
        return FindResult{true, &entry.name, &entry.resource};
      }
    }

    return FindResult{false, nullptr, nullptr};
  }

  class InsertResult {
  public:
    bool wasInserted() const {
      return m_wasInserted;
    }

    const nu::StringView& getName() const {
      return m_name;
    }

    ResourceType& getResource() const {
      return *m_resource;
    }

  private:
    friend class Cache;

    InsertResult(bool wasInserted, const nu::StringView& name, ResourceType* resource)
      : m_wasInserted{wasInserted}, m_name{name}, m_resource{resource} {}

    bool m_wasInserted;
    nu::StringView m_name;
    ResourceType* m_resource;
  };

  InsertResult insert(const nu::StringView& name, ResourceType resource) {
    Entry* result = m_entries.emplace({name, std::move(resource)});
    return InsertResult{true, result->name, &result->resource};
  }

private:
  struct Entry {
    nu::StaticString<128> name;
    ResourceType resource;
  };

  nu::GrowingArray<Entry> m_entries;

  DELETE_COPY_AND_MOVE(Cache);
};

}  // namespace hi

#endif  // HIVE_CACHE_H_
