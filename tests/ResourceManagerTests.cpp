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

  Resource<Data> load(hi::ResourceManager* resourceManager, nu::InputStream* inputStream) override {
    return Resource<Data>(nullptr);
  }

private:
  DELETE_COPY_AND_MOVE(DataResourceTypeManager);
};

struct Person : public nu::RefCounted<Person> {
  nu::StaticString<64> name;

  explicit Person(const nu::StringView& name) : name{name} {}
};

class PersonResourceTypeManager : public ResourceProcessor<Person> {
public:
  PersonResourceTypeManager() = default;

  Resource<Person> load(hi::ResourceManager* resourceManager,
                        nu::InputStream* inputStream) override {
    return Resource<Person>(nullptr);
  }

  //  Resource<Person> load(ResourceManager* resourceManager, nu::InputStream* inputStream) override {
//    LOG(Info) << "Loading person. (" << name << ")";
//    auto fileResource = resourceManager->get<File>(name);
//    if (!fileResource) {
//      return Resource<Person>{resourceManager};
//    }
//
//    nu::StaticString<64> personName;
//    NameProcessor np{&personName};
//    fileResource->process(&np);
//
//    auto person = nu::makeScopedRefPtr<Person>(personName);
//
//    return Resource<Person>(resourceManager, person);
//  }

private:
  struct NameProcessor : public FileProcessor {
    nu::Array<Char, 64> data;
    nu::StaticString<64>* name;

    void process(nu::InputStream* stream) override {
      auto bytesRead = stream->read(data.getData(), 64);
      name->append(data.getData(), bytesRead);
    }

    explicit NameProcessor(nu::StaticString<64>* name) : data{}, name(name) {}
  };
};

TEST_CASE("physical file resource") {
  ResourceManager rm;

  PhysicalFileResourceLocator locator{nu::getCurrentWorkingDirectory()};
  rm.addResourceLocatorBack(&locator);

  PhysicalFileResourceTypeManager pfrtm{nu::FilePath{__FILE__}.dirName() / "resources"};
  rm.registerResourceType(&pfrtm);

  PersonResourceTypeManager prtm;
  rm.registerResourceType(&prtm);

  auto person = rm.get<Person>("person.john.txt");

  CHECK(!person.isEmpty());
  CHECK(person->name.subString(0, 4) == "john");
}

TEST_CASE("manually created resource") {
  ResourceManager rm;
  DataResourceTypeManager dataResourceTypeManager;
  rm.registerResourceType(&dataResourceTypeManager);

  PhysicalFileResourceLocator locator{nu::getCurrentWorkingDirectory()};
  rm.addResourceLocatorBack(&locator);

  Resource<Data> dataResource{&rm, new Data{1, 2}};

  // rm.insert("data.1", dataResource);
}

}  // namespace hi
