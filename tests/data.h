#pragma once

#include <hive/in_memory_locator.h>
#include <hive/locator.h>
#include <nucleus/Memory/ScopedPtr.h>
#include <nucleus/Memory/ScopedRefPtr.h>
#include <nucleus/Streams/ArrayInputStream.h>
#include <nucleus/Streams/MemoryInputStream.h>

#include "hive/importer.h"

namespace hi {

struct Data {
  I32 a;
  I32 b;
};

class DataImporter : public Importer<Data> {
public:
  bool import(nu::StringView name, nu::InputStream* stream, Data* storage) override {
    storage->a = stream->readI32();
    storage->b = stream->readI32();
    return true;
  }
};

inline nu::ScopedPtr<nu::InputStream> stream_for_data(I32 a, I32 b) {
  Data d{a, b};

  return nu::makeScopedPtr<nu::MemoryInputStream>(&d, sizeof(d));
}

inline nu::ScopedRefPtr<LocatorSet> locator_set_for_data(nu::StringView name, I32 a, I32 b) {
  auto locator_set = nu::makeScopedRefPtr<LocatorSet>();

  Data dummy{a, b};
  nu::DynamicArray<U8> item{(U8*)&dummy, sizeof(dummy)};

  auto* locator = locator_set->register_locator<InMemoryLocator>();
  locator->set(name, std::move(item));

  return locator_set;
}

}  // namespace hi
