#ifndef HIVE_RESOURCE_TYPE_MANAGER_H_
#define HIVE_RESOURCE_TYPE_MANAGER_H_

#include "hive/Cache.h"
#include "hive/Resource.h"
#include "nucleus/Macros.h"
#include "nucleus/Text/StringView.h"
#include "nucleus/Types.h"

namespace hi {

using ResourceTypeId = MemSize;

template <typename T>
static ResourceTypeId typeIdFor() {
  static const char typeIdType = '\0';
  return reinterpret_cast<ResourceTypeId>(&typeIdType);
}

class ResourceProcessorBase {
public:
  virtual ~ResourceProcessorBase() = default;
};

template <typename ResourceType>
class ResourceProcessor : public ResourceProcessorBase {
public:
  ResourceProcessor() : m_id{typeIdFor<ResourceType>()} {}

  ~ResourceProcessor() override = default;

  ResourceTypeId id() const {
    return m_id;
  }

  virtual Resource<ResourceType> load(ResourceManager* resourceManager,
                                      nu::InputStream* inputStream) = 0;

private:
  DELETE_COPY_AND_MOVE(ResourceProcessor);

  friend class hi::ResourceManager;

  ResourceTypeId m_id;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_TYPE_MANAGER_H
