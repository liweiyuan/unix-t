#include <apue.h>

int main() {
  printf("hello world from process id: %ld\n", (long)getpid());
  exit(0);
}