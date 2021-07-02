#include <hive/in_memory_locator.h>

#include <catch2/catch.hpp>

#include "data.h"
#include "hive/resource_manager.h"

namespace hi {

TEST_CASE("ResourceManager") {
  const nu::ScopedRefPtr<InMemoryLocator>& locator = nu::make_scoped_ref_ptr<InMemoryLocator>();
  locator->set("data.txt", {1, 0, 0, 0, 2, 0, 0, 0, 0});

  ResourceManager rm{locator};

  SECTION("import resource type that is not registered") {
    auto resource = rm.import<Data>("data.txt");
    CHECK(resource.is_null());
  }

  SECTION("import resource that does not exist") {
    rm.register_importer<Data>("txt", nu::make_scoped_ptr<DataImporter>());
    auto resource = rm.import<Data>("data3.txt");
    CHECK(resource.is_null());
  }

  SECTION("import resource successfully") {
    rm.register_importer<Data>("txt", nu::make_scoped_ptr<DataImporter>());
    auto resource = rm.import<Data>("data.txt");
    CHECK(!resource.is_null());
    CHECK(resource->a == 1);
    CHECK(resource->b == 2);
  }
}

}  // namespace hi
