#ifndef HIVE_RESOURCE_H_
#define HIVE_RESOURCE_H_

#include "nucleus/Memory/ScopedRefPtr.h"

namespace hi {

class ResourceManager;

template <typename ResourceType>
class Resource : public nu::ScopedRefPtr<ResourceType> {
public:
  Resource(ResourceManager* resourceManager, ResourceType* resource)
    : nu::ScopedRefPtr<ResourceType>{resource}, m_resourceManager{resourceManager} {}
  virtual ~Resource() = default;

protected:
  ResourceManager* m_resourceManager;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_H_
