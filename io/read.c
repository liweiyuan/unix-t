#include <apue.h>
#include <stdio.h>

int main() {

  int data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

  FILE *file;
  file = fopen("/home/wade/Cursor-workspace/unix/fun/unix-t/testdata/read.txt",
               "w");
  if (fwrite(&data[2], sizeof(data), 4, file) != 4) {
    err_sys("fwrite error");
  }
  exit(0);
}