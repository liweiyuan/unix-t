#include "unix_file.h"

#define BUFFSIZE 4096
int main() {

  // 输出一个提示
  printf("请输入内容，输入exit退出\n");
  int n;
  char buf[BUFFSIZE];
  while ((n = read_file(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
    // 移除换行符
    if (n > 0 && buf[n - 1] == '\n') {
      buf[n - 1] = '\0';
      n--;
    }
    // 如果buf内容是exit则退出
    if (strcmp(buf, "exit") == 0) {
      break;
    }
    if (write_file(STDOUT_FILENO, buf, n) != n) {
      err_sys("write error");
    }
    // write输出后，换一个行
    if (write_file(STDOUT_FILENO, "\n", 1) != 1) {
      err_sys("write error");
    }
  }

  if (n < 0) {
    err_sys("read error");
    return -1;
  }
  return 0;
}