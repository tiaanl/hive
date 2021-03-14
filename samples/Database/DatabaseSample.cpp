
#include "hive/PhysicalResourceLocator.h"
#include "hive/ResourceManager.h"
#include "nucleus/Macros.h"

struct Employee {
  nu::StaticString<128> name;
  I32 age = 0;
};

class EmployeeResourceProcessor : public hi::Converter<Employee> {
public:
  bool load(hi::ResourceManager* NU_UNUSED(resourceManager), const nu::StringView& NU_UNUSED(name),
            nu::InputStream* inputStream, Employee* storage) override {
    {
      auto bytesRead =
          inputStream->readUntil(storage->name.data(), storage->name.capacity(), '\n');
      storage->name = nu::StringView{storage->name.data(), bytesRead - 1};
    }

    {
      nu::StaticString<64> temp;
      auto bytesRead = inputStream->readUntil(temp.data(), temp.capacity(), '\n');
      temp = nu::StringView{temp.data(), bytesRead - 1};
      storage->age = std::strtol(temp.data(), nullptr, 10);
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
  hi::ResourceManager resourceManager;

  auto rootPath = nu::FilePath{__FILE__}.dirName() / "data";
  hi::PhysicalFileResourceLocator physicalFileResourceLocator{rootPath};
  resourceManager.addResourceLocatorBack(&physicalFileResourceLocator);

  EmployeeResourceProcessor employeeResourceProcessor;
  resourceManager.registerConverter(&employeeResourceProcessor);

  printEmployee(resourceManager, "john.txt");
  printEmployee(resourceManager, "jane.txt");

  return 0;
}
