#ifndef HIVE_FILE_H_
#define HIVE_FILE_H_

#include "hive/ResourceProcessor.h"
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

class PhysicalFileResourceTypeManager : public ResourceProcessor<File> {
public:
  explicit PhysicalFileResourceTypeManager(
      const nu::FilePath& rootPath = nu::getCurrentWorkingDirectory())
    : ResourceProcessor{} {}

  ~PhysicalFileResourceTypeManager() override = default;

  Resource<File> load(hi::ResourceManager* resourceManager, nu::InputStream* inputStream) override {
    return Resource<File>(nullptr);
  }

  //  Resource<File> load(hi::ResourceManager* resourceManager, const nu::StringView& name) override
  //  {
  //    auto absolutePath = buildAbsolutePath(name);
  //    LOG(Info) << "Loading physical file: " << name << " (" << absolutePath << ")";
  //
  //    if (!nu::exists(absolutePath)) {
  //      LOG(Warning) << "File not found. (" << absolutePath << ")";
  //      return Resource<File>{resourceManager};
  //    }
  //
  //    return Resource<File>{resourceManager, new PhysicalFile{absolutePath}};
  //  }

private:
  DELETE_COPY_AND_MOVE(PhysicalFileResourceTypeManager);
};

}  // namespace hi

#endif  // HIVE_FILE_H_
