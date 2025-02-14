#include "unix_file.h"

int main() {
  int fd = open_file("file.txt", O_CREAT | O_RDWR);
  if (fd == -1) {
    exit(EXIT_FAILURE);
  }

  // 写入内容
  write_content(fd, "Hello, world!");

  // 将文件指针移动到文件末尾，准备追加写入
  off_t offset = lseek_file(fd, 0, SEEK_END);
  if (offset == -1) {
    close_file(fd);
    exit(EXIT_FAILURE);
  }

  // 写文件
  write_content(fd, " hi!");

  // 将文件指针移动到文件开头，准备再次读取
  offset = lseek_file(fd, 0, SEEK_SET);
  if (offset == -1) {
    close_file(fd);
    exit(EXIT_FAILURE);
  }

  // 读取文件内容
  read_content(fd);

  close_file(fd);
  // 删除文件
  delete_file("file.txt");

  return EXIT_SUCCESS;
}
