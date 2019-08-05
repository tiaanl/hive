#ifndef HIVE_TYPE_ID_H_
#define HIVE_TYPE_ID_H_

#include "nucleus/Types.h"

namespace hi {

using TypeId = MemSize;

template <typename T>
static TypeId typeIdFor() {
  static const char typeIdType = '\0';
  return reinterpret_cast<TypeId>(&typeIdType);
}

}  // namespace hi

#endif  // HIVE_TYPE_ID_H_
