#include "unix_file.h"

int main() {
  int fd = open_file2("copy.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    err_sys("open error");
    return 1;
  }

  // copy 文件描述符
  int newfd = fcntl(fd, F_DUPFD, 0);
  if (newfd < 0) {
    err_sys("fcntl error");
    return 1;
  }

  // 通过2个文件描述符写入数据
  write_file(fd, "Hello, ", 7);
  write_file(newfd, "World!\n", 7);

  close_file(fd);
  close_file(newfd);
  return 0;
}