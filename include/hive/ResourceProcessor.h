#ifndef HIVE_RESOURCE_TYPE_MANAGER_H_
#define HIVE_RESOURCE_TYPE_MANAGER_H_

#include "hive/Resource.h"
#include "nucleus/Macros.h"
#include "nucleus/Text/StringView.h"
#include "nucleus/Types.h"

namespace hi {

class ResourceProcessorBase {
public:
  virtual ~ResourceProcessorBase() = default;
};

template <typename T>
class ResourceProcessor : public ResourceProcessorBase {
public:
  using TypeId = MemSize;

  ResourceProcessor() : m_id{typeIdFor<T>()} {}

  ~ResourceProcessor() override = default;

  TypeId id() const {
    return m_id;
  }

  virtual hi::Resource<T> load(hi::ResourceManager* resourceManager,
                               nu::InputStream* inputStream) = 0;

private:
  DELETE_COPY_AND_MOVE(ResourceProcessor);

  friend class hi::ResourceManager;

  template <typename TypeIdType>
  static TypeId typeIdFor() {
    static const char typeIdType = '\0';
    return reinterpret_cast<MemSize>(&typeIdType);
  }

  TypeId m_id;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_TYPE_MANAGER_H
