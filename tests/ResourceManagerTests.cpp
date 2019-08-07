#include "hive/Converter.h"
#include "hive/ResourceManager.h"
#include "nucleus/Containers/Array.h"
#include "nucleus/Streams/WrappedMemoryInputStream.h"
#include "nucleus/Testing.h"
#include "nucleus/Text/StaticString.h"

namespace hi {

struct Data {
  U32 a;
  U32 b;
};

class DataResourceTypeManager : public Converter<Data> {
public:
  ~DataResourceTypeManager() override = default;

  bool load(ResourceManager*, nu::InputStream* inputStream, Data* storage) override {
    storage->a = inputStream->readU32();
    storage->b = inputStream->readU32();
    return true;
  }
};

class MockResourceLocator : public ResourceLocator {
public:
  bool process(const nu::StringView& name, Processor* processor) override {
    if (name == "data1") {
      nu::DynamicArray<U32> data;
      data.pushBack(1);
      data.pushBack(2);
      nu::WrappedMemoryInputStream stream{data.getData(), data.getSize() * sizeof(U32)};
      processor->process(&stream);
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
