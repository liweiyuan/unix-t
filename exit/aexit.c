#include <apue.h>

static void aexit1(void);
static void aexit2(void);

int main() {

  if (atexit(aexit1) != 0) {
    err_sys("atexit1 error..");
    return EXIT_FAILURE;
  }

  if (atexit(aexit1) != 0) {
    err_sys("atexit1 error...");
    return EXIT_FAILURE;
  }
  if (atexit(aexit2) != 0) {
    err_sys("atexit2 error");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

// atexit 依次会放到栈中

static void aexit1(void) { printf("aexit1 is called\n"); }

static void aexit2(void) { printf("aexit2 is called\n"); }