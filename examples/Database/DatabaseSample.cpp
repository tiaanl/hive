
#include "hive/ResourceManager.h"
#include "hive/physical_file_locator.h"
#include "nucleus/Text/StaticString.h"

struct Employee {
  nu::StaticString<128> name;
  I32 age = 0;
};

class EmployeeImporter : public hi::Importer<Employee> {
public:
  ~EmployeeImporter() override = default;

  bool import(hi::ResourceManager* resource_manager, nu::StringView name, nu::InputStream* stream,
              Employee* storage) override {
    {
      auto bytes_read = stream->readUntil(storage->name.data(), storage->name.capacity(), '\n');
      storage->name = nu::StringView{storage->name.data(), bytes_read - 1};
    }

    {
      nu::StaticString<64> temp;
      auto bytesRead = stream->readUntil(temp.data(), temp.capacity(), '\n');
      temp = nu::StringView{temp.data(), bytesRead - 1};
      storage->age = static_cast<decltype(storage->age)>(std::strtol(temp.data(), nullptr, 10));
    }

    return true;
  }
};

void printEmployee(hi::ResourceManager& resourceManager, const nu::StringView& name) {
  auto employee = resourceManager.get<Employee>(name);
  if (!employee) {
    LOG(Error) << "Employee not found. (" << name << ")";
    return;
  }

  LOG(Info) << "Employee: " << employee->name << " (" << employee->age << ")";
}

int main() {
  hi::ResourceManager rm;

  auto root_path = nu::getCurrentWorkingDirectory() / "data";
  rm.add_locator(nu::makeScopedPtr<hi::PhysicalFileLocator>(root_path));

  rm.register_importer<Employee>("txt", nu::makeScopedPtr<EmployeeImporter>());

  printEmployee(rm, "john.txt");
  printEmployee(rm, "jane.txt");

  return 0;
}
