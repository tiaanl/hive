
#include "hive/ResourceManager.h"
#include "hive/Resources/File.h"

namespace hi {

void registerDefaultResourceTypes(ResourceManager* resourceManager) {
  {
    static PhysicalFileResourceTypeManager tm;
    resourceManager->registerResourceProcessor<File>(&tm);
  }
}

}  // namespace hi
