#include <apue.h>
#include <stdio.h>

int main() {
  char name[L_tmpnam], line[MAXLINE];
  FILE *fp;

  printf("tmpnam() returns: %s\n", tmpnam(name));
  tmpnam(name);
  printf("%s\n", name);

  if ((fp = tmpfile()) == NULL) {
    err_sys("tmpfile error");
  }

  fputs("one line on temp file", fp);
  rewind(fp);
  if (fgets(line, sizeof(line), fp)== NULL) {
    err_sys("fgets error");
  }
  fputs(line, stdout);
  exit(0);
}