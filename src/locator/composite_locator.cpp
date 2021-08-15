#include "hive/locator/composite_locator.hpp"

namespace hi {

CompositeLocator::CompositeLocator(nu::DynamicArray<nu::ScopedRefPtr<Locator>> locators)
  : locators_{std::move(locators)} {}

CompositeLocator::~CompositeLocator() = default;

nu::ScopedPtr<nu::InputStream> CompositeLocator::locate(nu::StringView name) {
  for (auto& locator : locators_) {
    auto stream = locator->locate(name);
    if (stream) {
      return stream;
    }
  }
  return {};
}

}  // namespace hi
