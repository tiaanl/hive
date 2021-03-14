#include <catch2/catch.hpp>

#include "hive/Converter.h"
#include "hive/ResourceManager.h"
#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Streams/ArrayInputStream.h"
#include "nucleus/Streams/DynamicBufferOutputStream.h"
#include "nucleus/Streams/MemoryInputStream.h"

namespace hi {

struct Data {
  U32 a;
  U32 b;
};

class DataResourceTypeManager : public Converter<Data> {
public:
  ~DataResourceTypeManager() override = default;

  bool load(ResourceManager*, const nu::StringView&, nu::InputStream* inputStream,
            Data* storage) override {
    storage->a = inputStream->readU32();
    storage->b = inputStream->readU32();
    return true;
  }
};

class MockResourceLocator : public ResourceLocator {
public:
  bool process(nu::StringView name, Processor* processor) override {
    if (name == "data1") {
      nu::DynamicBufferOutputStream buffer;
      buffer.writeU32(1);
      buffer.writeU32(2);

      nu::MemoryInputStream stream{buffer.buffer()};

      processor->process(name, &stream);
      return true;
    }

    return false;
  }
};

TEST_CASE("basic fetching") {
  ResourceManager resourceManager;

  MockResourceLocator mockResourceLocator;
  resourceManager.addResourceLocatorBack(&mockResourceLocator);

  DataResourceTypeManager dataResourceTypeManager;
  resourceManager.registerConverter(&dataResourceTypeManager);

  Data* data1 = resourceManager.get<Data>("data1");
  CHECK(data1->a == 1);
  CHECK(data1->b == 2);

  Data* data2 = resourceManager.get<Data>("data1");
  CHECK(data2->a == 1);
  CHECK(data2->b == 2);
}

TEST_CASE("can get resources that was inserted without a locator") {
  ResourceManager resourceManager;

  DataResourceTypeManager dataResourceTypeManager;
  resourceManager.registerConverter(&dataResourceTypeManager);

  auto result = resourceManager.insert<Data>("data1", {1, 2});
  CHECK(result != nullptr);
  CHECK(result->a == 1);
  CHECK(result->b == 2);

  auto found = resourceManager.get<Data>("data1");
  CHECK(found != nullptr);
  CHECK(found->a == 1);
  CHECK(found->b == 2);
}

}  // namespace hi
