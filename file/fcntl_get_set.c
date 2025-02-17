#include "unix_file.h"

int main() {
  int fd = open_file2("file.txt", O_RDWR | O_CREAT, 0644);
  if (fd < 0) {
    err_sys("open error");
    return 1;
  }

  // 获取文件状态
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) {
    err_sys("fcntl error");
    close_file(fd);
    return 1;
  }
  printf("Current file status flags: %d\n", flags);

  // 设置文件状态
  int ret = fcntl(fd, F_SETFL, flags | O_APPEND);
  if (ret < 0) {
    err_sys("fcntl error");
    close_file(fd);
    return 1;
  }
  write_file(fd, "hello, ", 7);
  close_file(fd);
  return 0;
}