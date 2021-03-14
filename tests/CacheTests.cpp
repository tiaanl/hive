#include <catch2/catch.hpp>

#include "hive/Cache.h"

namespace hi {

struct Data {
  I32 a;
  I32 b;
};

TEST_CASE("can do something") {
  Cache<Data> cache;

  auto result = cache.insert("item1", {1, 2});
  CHECK(result.getName() == "item1");
  CHECK(result.getResource().a == 1);
  CHECK(result.getResource().b == 2);
}

TEST_CASE("find existing item in cache") {
  Cache<Data> cache;

  auto insertResult = cache.insert("item1", {1, 2});
  CHECK(insertResult.wasInserted());

  auto findResult = cache.find("item1");
  CHECK(findResult.found());
  CHECK(findResult.name() == "item1");
  CHECK(findResult.resource().a == 1);
  CHECK(findResult.resource().b == 2);

  auto notFoundResult = cache.find("item2");
  CHECK(!notFoundResult.found());
}

}  // namespace hi
