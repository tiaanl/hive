#pragma once

#include <hive/in_memory_locator.h>
#include <hive/locator.h>
#include <nucleus/Memory/scoped_ptr.h>
#include <nucleus/Memory/scoped_ref_ptr.h>
#include <nucleus/Streams/array_input_stream.h>
#include <nucleus/Streams/memory_input_stream.h>

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

inline nu::ScopedRefPtr<Locator> locator_for_data(nu::StringView name, I32 a, I32 b) {
  Data dummy{a, b};
  nu::DynamicArray<U8> item{(U8*)&dummy, sizeof(dummy)};

  auto locator = nu::makeScopedRefPtr<InMemoryLocator>();
  locator->set(name, std::move(item));

  return locator;
}

}  // namespace hi
