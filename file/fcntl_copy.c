#include <apue.h>
#include <fcntl.h>

int main() {
  int fd = open("copy.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
  write(fd, "Hello, ", 7);
  write(newfd, "World!\n", 7);

  close(fd);
  close(newfd);
  return 0;
}