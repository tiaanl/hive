#include <catch2/catch.hpp>

#include "data.h"
#include "hive/typed_resource.h"
#include "hive/typed_resource_cache.h"

namespace hi {

TEST_CASE("TypedResourceCache") {
  auto locator = locator_for_data("test.data", 10, 20);
  TypedResourceCache<Data> trc{std::move(locator)};
  trc.register_importer("data", nu::make_scoped_ptr<DataImporter>());

  SECTION("can get item") {
    auto* data = trc.get("test.data");
    REQUIRE(data != nullptr);
    CHECK(data->a == 10);
    CHECK(data->b == 20);
  }
}

}  // namespace hi
