#include <apue.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

/* open file*/
int open_file(const char *filename, int flags){
  int fd = open(filename, flags);
  if (fd == -1) {
    perror("open");
    return -1;
  }
  return fd;
}

/* lseek file*/
off_t lseek_file(int fd, off_t offset, int whence){
  off_t ret = lseek(fd, offset, whence);
  if (ret == -1) {
    perror("lseek");
    return -1;
  }
  return ret;
}

/* read file*/
ssize_t read_file(int fd, void *buf, size_t count){
  ssize_t n = read(fd, buf, count);
  if (n < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      printf("No data available for non-blocking read\n");
    } else {
      err_sys("read error");
      return -1;
    }
  }
  return n;
}

/* write file*/
ssize_t write_file(int fd, const void *buf, size_t count){
  ssize_t n = write(fd, buf, count);
  if (n < 0) {
    err_sys("write error");
    return -1;
  }
  return n;
}


/* 关闭文件*/
void close_file(int fd){
  if (close(fd) == -1) {
    perror("close");
  }
}

int main() {
  
  int fd = open_file("file.txt", O_RDWR);
  if (fd == -1) {
    return EXIT_FAILURE;
  }

  off_t offset = lseek_file(fd, 10, SEEK_SET);
  if (offset == -1) {
    perror("lseek");
    return EXIT_FAILURE;
  }
  printf("File offset set to %ld\n", offset);

  // 现在可以从偏移量10开始读写文件

  char buf[1024];
  ssize_t n;
  n = read(fd, buf, sizeof(buf));
  if (n < 0) {
    close_file(fd);
    return EXIT_FAILURE;
  } 
  //写文件
  n = write_file(fd, buf, n);
  if (n < 0) {
    close_file(fd);
    return EXIT_FAILURE;
  }
  close_file(fd);
  return EXIT_SUCCESS;
}