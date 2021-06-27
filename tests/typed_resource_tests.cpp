#include <nucleus/types.h>

#include <catch2/catch.hpp>

#include "data.h"
#include "hive/typed_resource.h"

namespace hi {

TEST_CASE("TypedResource") {
  SECTION("importers") {
    TypedResource<Data> tr{{}};

    tr.register_importer("data", nu::make_scoped_ptr<DataImporter>());
    CHECK(tr.importer_for_extension("data"));
    CHECK(!tr.importer_for_extension("test"));
  }

  SECTION("import") {
    auto locator_set = locator_for_data("item.data", 10, 20);
    TypedResource<Data> tr{std::move(locator_set)};

    auto stream = stream_for_data(10, 20);

    tr.register_importer("data", nu::make_scoped_ptr<DataImporter>());

    auto resource = tr.import("item.data");
    REQUIRE(!resource.is_null());
    CHECK(resource->a == 10);
    CHECK(resource->b == 20);
  }
}

}  // namespace hi
