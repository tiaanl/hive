#ifndef HIVE_CACHE_H_
#define HIVE_CACHE_H_

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Memory/ScopedPtr.h"
#include "nucleus/Text/StaticString.h"

namespace hi {

template <typename ResourceType>
class Cache {
public:
  NU_DELETE_COPY_AND_MOVE(Cache);

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
    for (auto& entry : m_entries) {
      if (entry->name == name) {
        return FindResult{true, &entry->name, &(entry->resource)};
      }
    }

    return FindResult{false, nullptr, nullptr};
  }

  class InsertResult {
  public:
    bool wasInserted() const {
      return m_wasInserted;
    }

    nu::StringView getName() const {
      return m_name;
    }

    ResourceType& getResource() {
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

  InsertResult insert(nu::StringView name, ResourceType resource) {
    auto result = m_entries.emplaceBack(new Entry(name, std::move(resource)));
    return {true, name, &result.element()->resource};
  }

  template <typename... Args>
  InsertResult emplace(const nu::StringView& name, Args&&... args) {
    Entry* result = m_entries.emplace(name, std::forward<Args>(args)...);
    return InsertResult(true, result->name, &result->resource);
  }

private:
  struct Entry {
    nu::StaticString<128> name;
    ResourceType resource;

    Entry() : name{}, resource{} {}
    Entry(nu::StringView name, ResourceType&& resource)
      : name{name}, resource{std::forward<ResourceType>(resource)} {}
  };

  nu::DynamicArray<nu::ScopedPtr<Entry>> m_entries;
};

}  // namespace hi

#endif  // HIVE_CACHE_H_
