#ifndef HIVE_RESOURCE_TYPE_MANAGER_H_
#define HIVE_RESOURCE_TYPE_MANAGER_H_

#include "nucleus/Streams/InputStream.h"

namespace hi {

class ResourceManager;

class ConverterBase {
public:
  virtual ~ConverterBase();
};

template <typename ResourceType>
class Converter : public ConverterBase {
public:
  Converter() = default;
  ~Converter() override = default;

  virtual bool load(ResourceManager* resourceManager, nu::InputStream* inputStream,
                    ResourceType* storage) = 0;

private:
  DELETE_COPY_AND_MOVE(Converter);

  friend class hi::ResourceManager;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_TYPE_MANAGER_H
