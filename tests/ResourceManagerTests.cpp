#include "hive/ResourceManager.h"
#include "nucleus/Testing.h"

namespace hi {

TEST_CASE("basic resource management") {
  ResourceManager rm{nullptr, nu::FilePath{__FILE__}.dirName().dirName()};
  auto file = rm.getFile("test.ini");
}

}  // namespace hi
