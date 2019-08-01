#include "hive/ResourceManager.h"
#include "hive/ResourceProcessor.h"
#include "hive/Resources/File.h"
#include "nucleus/Containers/Array.h"
#include "nucleus/RefCounted.h"
#include "nucleus/Testing.h"
#include "nucleus/Text/StaticString.h"

namespace hi {

struct Data : public nu::RefCounted<Data> {
  U32 a;
  U32 b;

  Data(U32 a, U32 b) : a{a}, b{b} {}
};

class DataResourceTypeManager : public ResourceProcessor<Data> {
public:
  DataResourceTypeManager() = default;

  ~DataResourceTypeManager() override = default;

  Resource<Data> load(hi::ResourceManager*, nu::InputStream*) override {
    return Resource<Data>(nullptr);
  }

private:
  DELETE_COPY_AND_MOVE(DataResourceTypeManager);
};

struct Person : public nu::RefCounted<Person> {
  nu::StaticString<64> name;

  explicit Person(const nu::StringView& name) : name{name} {}
};

class PersonResourceProcessor : public ResourceProcessor<Person> {
public:
  PersonResourceProcessor() = default;

  Resource<Person> load(hi::ResourceManager* resourceManager,
                        nu::InputStream* inputStream) override {
    nu::Array<Char, 64> data;
    auto bytesRead = inputStream->read(data.getData(), 64);
    data[bytesRead] = 0;
    
    return Resource<Person>(resourceManager, new Person{data.getData()});
  }
};

TEST_CASE("physical file resource") {
  ResourceManager rm;

  PhysicalFileResourceLocator locator{nu::FilePath{__FILE__}.dirName() / "resources"};
  rm.addResourceLocatorBack(&locator);

  PersonResourceProcessor personResourceProcessor;
  rm.registerResourceProcessor(&personResourceProcessor);

  auto person = rm.get<Person>("person.john.txt");

  CHECK(!person.isEmpty());
  CHECK(person->name.subString(0, 4) == "john");
}

TEST_CASE("manually created resource") {
  ResourceManager rm;
  DataResourceTypeManager dataResourceTypeManager;
  rm.registerResourceProcessor(&dataResourceTypeManager);

  PhysicalFileResourceLocator locator{nu::getCurrentWorkingDirectory()};
  rm.addResourceLocatorBack(&locator);

  Resource<Data> dataResource{&rm, new Data{1, 2}};

  rm.add("data.1", dataResource);

  auto data1 = rm.get<Data>("data.1");

  CHECK(data1->a == 1);
  CHECK(data1->b == 2);

  auto data1again = rm.get<Data>("data.1");

  // We should be the same object as we expect it to come from the cache.
  CHECK(data1.get() == data1again.get());
}

}  // namespace hi
