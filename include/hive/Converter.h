#ifndef HIVE_RESOURCE_TYPE_MANAGER_H_
#define HIVE_RESOURCE_TYPE_MANAGER_H_

#include "nucleus/Streams/InputStream.h"
#include "nucleus/Text/StringView.h"

namespace hi {

class ResourceManager;

class ConverterBase {
public:
  virtual ~ConverterBase();
};

template <typename ResourceType>
class Converter : public ConverterBase {
public:
  NU_DELETE_COPY_AND_MOVE(Converter);

  Converter() = default;
  ~Converter() override = default;

  virtual bool load(ResourceManager* resourceManager, const nu::StringView& name,
                    nu::InputStream* inputStream, ResourceType* storage) = 0;

private:
  friend class hi::ResourceManager;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_TYPE_MANAGER_H
