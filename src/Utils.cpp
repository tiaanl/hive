
#include "hive/ResourceManager.h"
#include "hive/Resources/File.h"

namespace hi {

void registerDefaultResourceTypes(ResourceManager* resourceManager) {
  {
    static PhysicalFileResourceTypeManager tm;
    resourceManager->registerResourceType<File>(&tm);
  }
}

}  // namespace hi
