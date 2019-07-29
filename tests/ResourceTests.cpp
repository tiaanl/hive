#include "nucleus/Testing.h"

#include "hive/Resource.h"
#include "hive/ResourceManager.h"
#include "nucleus/Types.h"

namespace hi {

struct Person : public nu::RefCounted<Person> {
  U32 age = 0;
};

TEST_CASE("resource") {
  ResourceManager resourceManager{nullptr};
  Resource<Person> p1{&resourceManager, new Person};
}

}  // namespace hi
