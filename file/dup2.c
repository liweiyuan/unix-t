#include "unix_file.h"

int main() {
  int fd = open_file2("file-dup2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    return 1;
  }
  int copy_fd2 = dup2_file(fd, STDOUT_FILENO);
  if (copy_fd2 < 0) {
    return 1;
  }
  printf("This will be written to file\n");
  close_file(fd);
  return 0;
}
