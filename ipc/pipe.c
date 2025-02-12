#include <apue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define DEF_PAPER "/bin/more"

int main(int argc, char *argv[]) {

  int n;
  int fd[2];
  pid_t pid;
  char *paper, *agrv0;
  char line[MAXLINE];
  FILE *fp;

  if (argc != 2) {
    err_quit("usage: pipe <filename>");
  }

  if ((fp = fopen(argv[1], "r")) == NULL) {
    err_sys("can't open %s", argv[1]);
  }
  if (pipe(fd) < 0) {
    err_sys("pipe error");
  }

  // 创建子进程
  if ((pid = fork()) < 0) {
    err_sys("fork error");
  } else if (pid > 0) {
    // parent
    close(fd[0]); // 关闭read
    while (fgets(line, MAXLINE, fp) != NULL) {
      n = strlen(line);
      // write
      if (write(fd[1], line, n) != n) {
        err_sys("write error to pipe");
      }
    }

    if (ferror(fp)) {
      err_sys("fgets error");
    }
    close(fd[1]); // 关闭write

    // wait 子进程
    if (waitpid(pid, NULL, 0) < 0) {
      err_sys("waitpid error");
    }
    exit(0);
  } else {
    // child
    close(fd[1]);
    if (fd[0] != STDIN_FILENO) {
      if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO) {
        err_sys("dup2 error to stdin");
        close(fd[0]);
      }

      if ((paper = getenv("PAGER")) == NULL) {
        paper = DEF_PAPER;
      }
      if ((agrv0 = strrchr(paper, '/')) != NULL) {
        agrv0++;
      } else {
        agrv0 = paper;
      }

      if (execl(paper, agrv0, (char *)0) < 0) {
        err_sys("execl error for %s", paper);
      }
    }
    close(fd[0]);
    exit(0);
  }
  return 0;
}