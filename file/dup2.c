#include <apue.h>
#include <fcntl.h>

int main() {

  int fd = open("file-dup2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    err_sys("open error");
    return 1;
  }

  int copy_fd2 = dup2(fd, STDOUT_FILENO);
  if (copy_fd2 < 0) {
    err_sys("dup2 error");
    return 1;
  }

  printf("This will be written to file\n");
  close(fd);
  return 0;
}
