#include "unix_file.h"
#include <apue.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

/* open file*/
int open_file(const char *filename, int flags) {
  int fd = open(filename, flags, 0644);
  if (fd == -1) {
    perror("open");
    return -1;
  }
  return fd;
}

/* lseek file*/
off_t lseek_file(int fd, off_t offset, int whence) {
  off_t ret = lseek(fd, offset, whence);
  if (ret == -1) {
    perror("lseek");
    return -1;
  }
  return ret;
}

/* read file*/
ssize_t read_file(int fd, void *buf, size_t count) {
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
ssize_t write_file(int fd, const void *buf, size_t count) {
  ssize_t n = write(fd, buf, count);
  if (n < 0) {
    err_sys("write error");
    return -1;
  }
  return n;
}

/* 关闭文件*/
void close_file(int fd) {
  if (close(fd) == -1) {
    perror("close");
  }
}

/* 写入文件内容*/
void write_content(int fd, const char *content) {
  ssize_t n = write_file(fd, content, strlen(content));
  if (n < 0) {
    close_file(fd);
    exit(EXIT_FAILURE);
  }
}

/* 读取文件内容*/
void read_content(int fd) {
  char buf[1024];
  ssize_t n = read_file(fd, buf, sizeof(buf));
  if (n < 0) {
    close_file(fd);
    exit(EXIT_FAILURE);
  }
  printf("Read %zd bytes from file\n", n);
  buf[n] = '\0';
  printf("Read data: %s\n", buf);
}

/* 删除文件*/
void delete_file(const char *filename) {
  if (unlink(filename) == -1) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }
}