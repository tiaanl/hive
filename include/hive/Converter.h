#ifndef HIVE_RESOURCE_TYPE_MANAGER_H_
#define HIVE_RESOURCE_TYPE_MANAGER_H_

#include "hive/TypeId.h"
#include "nucleus/Streams/InputStream.h"

namespace hi {

class ResourceManager;

class ConverterBase {
public:
  virtual ~ConverterBase() = default;
};

template <typename ResourceType>
class Converter : public ConverterBase {
public:
  Converter() : m_id{typeIdFor<ResourceType>()} {}

  ~Converter() override = default;

  TypeId id() const {
    return m_id;
  }

  virtual bool load(ResourceManager* resourceManager, nu::InputStream* inputStream,
                    ResourceType* storage) = 0;

private:
  DELETE_COPY_AND_MOVE(Converter);

  friend class hi::ResourceManager;

  TypeId m_id;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_TYPE_MANAGER_H
