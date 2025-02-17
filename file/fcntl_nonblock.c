#include "unix_file.h"
#include <errno.h>
#include <fcntl.h>

int main() {
  int fd = open_file2("file.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
  if (fd < 0) {
    err_sys("open error");
    return 1;
  }

  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) {
    err_sys("fcntl error");
    return 1;
  }

  // 设置为非阻塞
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
    err_sys("fcntl nonblock error");
    return 1;
  }

  char buf[1024];
  ssize_t n;
  n = read_file(fd, buf, sizeof(buf));
  if (n < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      printf("No data available for non-blocking read\n");
    } else {
      err_sys("read error");
      return 1;
    }
  } else {
    write_file(STDOUT_FILENO, buf, n);
  }
  close_file(fd);
  return 0;
}