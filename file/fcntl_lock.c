#include <apue.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main() {
  int fd = open("file.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
  if (fd < 0) {
    err_sys("open error");
    return 1;
  }

  struct flock lock;
  memset(&lock, 0, sizeof(lock));
  lock.l_type = F_WRLCK;    // 写锁
  lock.l_whence = SEEK_SET; //
  lock.l_start = 0;
  lock.l_len = 0; // 锁定整个文件

  if (fcntl(fd, F_SETLK, &lock) < 0) { // 获取写锁， 阻塞直到成功
    err_sys("lock error: %d", errno);
    return 1;
  }

  printf("Write lock acquired\n");

  // 进行写操作...
  write(fd, "Hello, World!\n", 14);
  lock.l_type = F_UNLCK; // 释放锁

  if (fcntl(fd, F_SETLK, &lock) < 0) {
    err_sys("lock error: %d", errno);
    return 1;
  }
  printf("Write lock released\n");
  close(fd);
  return 0;
}