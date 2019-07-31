#ifndef HIVE_RESOURCE_H_
#define HIVE_RESOURCE_H_

#include "nucleus/Logging.h"
#include "nucleus/Memory/ScopedRefPtr.h"

namespace hi {

class ResourceManager;

template <typename ResourceType>
class Resource : public nu::ScopedRefPtr<ResourceType> {
public:
  explicit Resource(ResourceManager* resourceManager, ResourceType* resource = nullptr)
    : nu::ScopedRefPtr<ResourceType>{resource}, m_resourceManager{resourceManager} {
    DCHECK(m_resourceManager != nullptr);
  }

  virtual ~Resource() = default;

protected:
  ResourceManager* m_resourceManager;
};

}  // namespace hi

#endif  // HIVE_RESOURCE_H_
