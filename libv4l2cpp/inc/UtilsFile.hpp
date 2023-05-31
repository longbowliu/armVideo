#ifndef FILE_HPP
#define FILE_HPP

#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

namespace utils_file {
enum class FileMode {
  EXISTS = 0,   //存在
  EXECUTE = 1,  //执行
  WRITE = 2,    //写
  READ = 4      //读
};

/**
 * 文件操作
 * @author: sherlock_lht
 */
class BaseFileIO {
 protected:
  std::fstream fileStream;
  std::string fileName;

  explicit BaseFileIO(const std::string &fileName) {
    this->fileName = fileName;
  }

 public:
  /**
   * 检查文件是否存在
   */
  static bool exists(const std::string &filePath) {
    return access(filePath.c_str(), F_OK) == 0;
  }

  /**
   * 检查指定文件的指定属性，包括是否存在、是否可读、是否可写、是否有权限执行
   */
  static bool checkFileMode(const std::string &filePath,
                            utils_file::FileMode mode) {
    return access(filePath.c_str(), (int)mode) == 0;
  }

  /**
   * 给指定文件赋予权限，同chmod指令
   */
  static bool chmodFile(const std::string &filePath, int mode) {
    return chmod(filePath.c_str(), mode) == 0;
  }

 public:
  /**
   * 打开文件，返回是否成功打开
   */
  bool open(std::ios_base::openmode mode) {
    if (fileStream.is_open()) {
      close();
    }
    fileStream.open(fileName, mode);
    return fileStream.is_open();
  }

  /**
   * 设置文件的开始读取偏移
   */
  void seekInputPosition(std::size_t offsetPosition,
                         std::ios_base::seekdir position = std::ios::beg) {
    fileStream.seekg(offsetPosition, position);
  }

  /**
   * 是否已经读到end，可用此方法判断文件是否已经读完
   */
  bool endOfFile() const { return fileStream.eof(); }

  /**
   * 获取当前处理的文件名称
   */
  std::string getFileName() const { return fileName; }

  /**
   * 关闭文件流，如果从未打开，则无任何操作
   */
  void close() {
    if (fileStream.is_open()) {
      fileStream.close();
    }
  }

  virtual ~BaseFileIO() { close(); }
};

class File : public BaseFileIO {
 public:
  /**
   * 向指定的文件写入指定的内容，返回值表示是否写入成功
   */
  static bool saveTextTo(const std::string &fileName, const std::string &text) {
    File fileToWrite(fileName);
    if (fileToWrite.open(std::ios::out | std::ios::app)) {
      fileToWrite.write(text);
      fileToWrite.close();
      return true;
    }
    return false;
  }

 public:
  explicit File(const std::string &fileName) : BaseFileIO(fileName) {}

  /**
   * 写入内容，返回值表示写入成功的长度
   */
  std::size_t write(const std::string &content) {
    auto before = fileStream.tellp();
    fileStream.write(content.data(), content.length());
    return fileStream.tellp() - before;
  }

  /**
   * 读取所有文件内容
   * offset参数表示开始读取的偏移
   * 如果在调用之前有读取过文件，则会影响该函数的开始读取位置
   */
  std::string readAll(std::size_t offset = 0) {
    if (!fileStream.is_open()) {
      return "";
    }
    if (offset > 0) {
      seekInputPosition(offset);
    }
    std::stringstream buf;
    buf << fileStream.rdbuf();
    return buf.str();
  }

  /**
   * 读取指定数量的字符串，如果内容不够，则读完
   * offset参数表示开始读取的偏移
   * 读取位置置于本次最后读取的内容的结尾
   */
  std::string read(std::size_t maxSize, std::size_t offset = 0) {
    if (!fileStream.is_open()) {
      return "";
    }
    if (offset > 0) {
      seekInputPosition(offset);
    }
    std::unique_ptr<char[]> buf(new (std::nothrow) char[maxSize + 1]);
    fileStream.read(buf.get(), maxSize);
    std::size_t size = fileStream.gcount();
    std::string str(buf.get(), size);
    return str;
  }

  /**
   * 从标记开始位置读取到一行结尾，包括终止符
   * offset参数表示开始读取的偏移
   */
  std::string readLine(std::size_t maxSize, std::size_t offset = 0) {
    if (!fileStream.is_open()) {
      return "";
    }

    std::unique_ptr<char[]> buf(new (std::nothrow) char[maxSize + 1]);
    fileStream.getline(buf.get(), maxSize);
    std::size_t size = fileStream.gcount();
    std::string str(buf.get(), size);
    return str;
  }
};
}  // namespace utils_file

#endif  // FILE_HPP
