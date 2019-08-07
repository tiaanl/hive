
#include "hive/PhysicalResourceLocator.h"
#include "hive/ResourceManager.h"

struct Employee {
  nu::StaticString<128> name;
  I32 age = 0;
};

class EmployeeResourceProcessor : public hi::Converter<Employee> {
public:
  bool load(hi::ResourceManager*, nu::InputStream* inputStream, Employee* storage) override {
    {
      auto bytesRead =
          inputStream->readUntil(storage->name.getData(), storage->name.getStorageSize(), '\n');
      storage->name = nu::StringView(storage->name, bytesRead - 1);
    }

    {
      nu::StaticString<64> temp;
      auto bytesRead = inputStream->readUntil(temp.getData(), temp.getStorageSize(), '\n');
      temp = nu::StringView(temp, bytesRead - 1);
      storage->age = std::strtol(temp.getData(), nullptr, 10);
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
  printEmployee(resourceManager, "sarah.txt");

  return 0;
}
