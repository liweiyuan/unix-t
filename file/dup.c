#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  // 打开文件
  int fd = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    perror("open");
    return 1;
  }

  // 复制文件描述符
  int fd_copy = dup(fd);
  if (fd_copy < 0) {
    perror("dup");
    return 1;
  }

  // 使用fd写入文件
  const char *msg1 = "This is written by fd\n";
  ssize_t bytes_written = write(fd, msg1, strlen(msg1));
  if (bytes_written < 0) {
    perror("write");
    return 1;
  }

  // 使用fd_copy写入文件
  const char *msg2 = "This is written by fd_copy\n";
  bytes_written = write(fd_copy, msg2, strlen(msg2));
  if (bytes_written < 0) {
    perror("write");
    return 1;
  }

  // 关闭文件描述符
  close(fd);
  close(fd_copy);

  return 0;
}