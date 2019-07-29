#ifndef HIVE_FILE_H_
#define HIVE_FILE_H_

#include "nucleus/FilePath.h"
#include "nucleus/RefCounted.h"
#include "nucleus/Streams/FileInputStream.h"

namespace hi {

class FileProcessor {
public:
  virtual ~FileProcessor() = default;

  virtual void process(nu::InputStream* stream) = 0;
};

class File : public nu::RefCounted<File> {
public:
  File() = default;
  virtual ~File() = default;

  virtual void process(FileProcessor* fileProcessor) = 0;

private:
  DELETE_COPY_AND_MOVE(File);
};

class PhysicalFile : public File {
public:
  explicit PhysicalFile(const nu::FilePath& path) : m_path{path} {}

  ~PhysicalFile() override = default;

  void process(FileProcessor* fileProcessor) override {
    nu::FileInputStream stream{m_path};
    fileProcessor->process(&stream);
  }

private:
  DELETE_COPY_AND_MOVE(PhysicalFile);

  nu::FilePath m_path;
};

}  // namespace hi

#endif  // HIVE_FILE_H_
