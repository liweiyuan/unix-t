#include "unix_file.h"

int main() {
  int fd = open_file2("file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (fd < 0) {
    err_sys("open error");
    exit(1);
  }

  const char *str = "This is some data to be written to the file.";
  if (write_file(fd, str, strlen(str)) < 0) {
    err_sys("write error");
    exit(1);
  }

  // 使用fsync刷新数据到磁盘
  if (fsync(fd) < 0) {
    err_sys("fsync error");
    exit(1);
  }

  // 使用fdatasync刷新数据
  if (fdatasync(fd) < 0) {
    err_sys("fdatasync error");
    exit(1);
  }

  close_file(fd);
  return 0;
}