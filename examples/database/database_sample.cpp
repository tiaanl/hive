#include <hive/importer.h>
#include <hive/physical_file_locator.h>
#include <hive/resource_manager.h>
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

  bool import(nu::StringView NU_UNUSED(name), nu::InputStream* stream, Employee* storage) override {
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

void print_employee(const Employee& employee) {
  LOG(Info) << "Employee: " << employee.name << " (" << employee.age << ")";
}

int main() {
  auto root_path = nu::getCurrentWorkingDirectory() / "data";

  const nu::ScopedRefPtr<hi::PhysicalFileLocator>& locator =
      nu::make_scoped_ref_ptr<hi::PhysicalFileLocator>(root_path);

  hi::ResourceManager rm{locator};
  rm.register_importer<Employee>("txt", nu::make_scoped_ptr<EmployeeImporter>());

  auto john = rm.import<Employee>("john.txt");
  auto jane = rm.import<Employee>("jane.txt");

  print_employee(*john);
  print_employee(*jane);

  return 0;
}
