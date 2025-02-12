#define __USE_GNU
#include <apue.h>

int main() {

  char **envir = __environ;
  while (*envir) {
    fprintf(stdout, "%s\n", *envir);
    envir++;
  }
  return EXIT_SUCCESS;
}
