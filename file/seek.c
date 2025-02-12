#include <apue.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int fd = open("file.txt", O_RDWR);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  off_t offset = lseek(fd, 10, SEEK_SET);
  if (offset == -1) {
    perror("lseek");
    return 1;
  }

  printf("File offset set to %ld\n", offset);

  // 现在可以从偏移量10开始读写文件

  char buf[1024];
  ssize_t n;
  n = read(fd, buf, sizeof(buf));
  if (n < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      printf("No data available for non-blocking read\n");
    } else {
      err_sys("read error");
      return 1;
    }
  } else {
    write(STDOUT_FILENO, buf, n);
  }
  close(fd);
  return 0;
}