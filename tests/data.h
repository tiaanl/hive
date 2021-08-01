#pragma once

#include <hive/locator/in_memory_locator.h>
#include <hive/locator/locator.h>
#include <nucleus/memory/scoped_ptr.h>
#include <nucleus/memory/scoped_ref_ptr.h>
#include <nucleus/streams/array_input_stream.h>
#include <nucleus/streams/memory_input_stream.h>

#include "hive/importer/importer.h"

namespace hi {

struct Data {
  I32 a;
  I32 b;
};

class DataImporter : public Importer<Data> {
public:
  bool import(nu::StringView NU_UNUSED(name), nu::InputStream* stream, Data* storage) override {
    storage->a = stream->readI32();
    storage->b = stream->readI32();
    return true;
  }
};

inline nu::ScopedPtr<nu::InputStream> stream_for_data(I32 a, I32 b) {
  Data d{a, b};

  return nu::make_scoped_ptr<nu::MemoryInputStream>(&d, sizeof(d));
}

inline nu::ScopedRefPtr<Locator> locator_for_data(nu::StringView name, I32 a, I32 b) {
  Data dummy{a, b};
  nu::DynamicArray<U8> item{(U8*)&dummy, sizeof(dummy)};

  auto locator = nu::make_scoped_ref_ptr<InMemoryLocator>();
  locator->set(name, std::move(item));

  return locator;
}

}  // namespace hi
