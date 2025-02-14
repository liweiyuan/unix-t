#include "unix_file.h"

int main() {
  // 打开文件
  int fd = open_file2("file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    return 1;
  }

  // 复制文件描述符
  int fd_copy = dup_file(fd);
  if (fd_copy < 0) {
    return 1;
  }

  // 使用fd写入文件
  const char *msg1 = "This is written by fd\n";
  ssize_t bytes_written = write_file(fd, msg1, strlen(msg1));
  if (bytes_written < 0) {
    return 1;
  }

  // 使用fd_copy写入文件
  const char *msg2 = "This is written by fd_copy\n";
  bytes_written = write_file(fd_copy, msg2, strlen(msg2));
  if (bytes_written < 0) {

    return 1;
  }

  // 关闭文件描述符
  close_file(fd);
  close_file(fd_copy);

  return 0;
}