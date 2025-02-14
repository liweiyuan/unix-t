#include "unix_file.h"

int main() {
  int fd = open_file2("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    err_sys("open error");
    return 1;
  }

  // 复制标准输出描述符
  int stdout_fd = dup_file(STDOUT_FILENO);
  if (stdout_fd < 0) {
    err_sys("dup error");
    return 1;
  }

  // 将标准输出重定向到文件描述符fd
  int result = dup2_file(fd, STDOUT_FILENO);
  if (result < 0) {
    err_sys("dup2 error");
    return 1;
  }

  // write
  printf("This will be written to both console and output.txt file\n");
  // 刷新标准输出缓冲区，确保数据写入文件
  fflush(stdout);

  // 恢复标准输出
  dup2_file(stdout_fd, STDOUT_FILENO);
  close_file(stdout_fd);
  fflush(stdout);

  close_file(fd);
  return 0;
}
