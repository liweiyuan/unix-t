#include <apue.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    err_sys("open error");
    return 1;
  }

  // 复制标准输出描述符
  int stdout_fd = dup(STDOUT_FILENO);

  // 将标准输出重定向到文件描述符fd
  int result = dup2(fd, STDOUT_FILENO);
  if (result < 0) {
    err_sys("dup2 error");
    return 1;
  }

  // write
  printf("This will be written to both console and output.txt file\n");
  // 恢复标准输出
  dup2(stdout_fd, STDOUT_FILENO);
  close(stdout_fd);
  fflush(stdout);

  close(fd);
  return 0;
}
