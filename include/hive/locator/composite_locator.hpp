#pragma once

#include <nucleus/macros.h>
#include <nucleus/memory/scoped_ref_ptr.h>

#include "hive/locator/locator.h"

namespace hi {

class CompositeLocator : public Locator {
  NU_DELETE_COPY_AND_MOVE(CompositeLocator);

public:
  explicit CompositeLocator(nu::DynamicArray<nu::ScopedRefPtr<Locator>> locators);
  ~CompositeLocator() override;

  nu::ScopedPtr<nu::InputStream> locate(nu::StringView name) override;

private:
  nu::DynamicArray<nu::ScopedRefPtr<Locator>> locators_;
};

}  // namespace hi
