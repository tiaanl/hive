#include <hive/importer.h>
#include <hive/physical_file_locator.h>
#include <hive/typed_resource.h>
#include <nucleus/file_path.h>
#include <nucleus/text/static_string.h>

struct Employee {
  nu::StaticString<128> name;
  I32 age = 0;
};

class EmployeeImporter : public hi::Importer<Employee> {
public:
  ~EmployeeImporter() override = default;

  bool import(nu::StringView name, nu::InputStream* stream, Employee* storage) override {
    {
      auto bytes_read = stream->readUntil(storage->name.data(), storage->name.capacity(), '\n');
      storage->name = nu::StringView{storage->name.data(), bytes_read - 1};
    }

    {
      nu::StaticString<64> temp;
      auto bytes_read = stream->readUntil(temp.data(), temp.capacity(), '\n');
      temp = nu::StringView{temp.data(), bytes_read - 1};
      storage->age = static_cast<decltype(storage->age)>(std::strtol(temp.data(), nullptr, 10));
    }

    return true;
  }
};

void print_employee(hi::TypedResource<Employee>& employees, nu::StringView name) {
  auto employee = employees.import(name);
  if (!employee) {
    LOG(Error) << "Employee not found. (" << name << ")";
    return;
  }

  LOG(Info) << "Employee: " << employee->name << " (" << employee->age << ")";
}

int main() {
  auto root_path = nu::getCurrentWorkingDirectory() / "data";

  hi::TypedResource<Employee> employees{
      nu::make_scoped_ref_ptr<hi::PhysicalFileLocator>(root_path)};
  employees.register_importer("txt", nu::make_scoped_ptr<EmployeeImporter>());

  print_employee(employees, "john.txt");
  print_employee(employees, "jane.txt");

  return 0;
}
