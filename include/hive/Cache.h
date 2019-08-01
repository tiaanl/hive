#ifndef HIVE_CACHE_H_
#define HIVE_CACHE_H_

#include "hive/Resource.h"
#include "nucleus/Macros.h"
#include "nucleus/Text/StaticString.h"

#include <map>

namespace hi {

class CacheBase {
public:
  virtual ~CacheBase() = default;
};

template <typename ResourceType>
class Cache : public CacheBase {
public:
  explicit Cache(ResourceManager* resourceManager) : m_resourceManager{resourceManager} {}

  void add(const nu::StringView& name, const Resource<ResourceType>& resource) {
    m_cache[name] = resource;
  }

  Resource<ResourceType> get(const nu::StringView& name) {
    auto result = m_cache.find(name);
    if (result == result.end()) {
      return Resource<ResourceType>{m_resourceManager, nullptr};
    }

    return result.second;
  }

private:
  DELETE_COPY_AND_MOVE(Cache);

  ResourceManager* m_resourceManager;
  std::map<nu::StaticString<128>, Resource<ResourceType>> m_cache;
};

}  // namespace hi

#endif  // HIVE_CACHE_H_
