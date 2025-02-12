#include <apue.h>
#include <sys/resource.h>

#define doit(name) pr_limit(#name, name)

static void pr_limit(char *, int);

typedef struct rlimit rt;

int main(void) {
#ifdef RLIMIT_AS
  doit(RLIMIT_AS);
#endif
  doit(RLIMIT_CORE);
  doit(RLIMIT_CPU);
  doit(RLIMIT_DATA);
  doit(RLIMIT_FSIZE);

  //......
  return 0;
}

static void pr_limit(char *name, int resource) {
  rt limit;
  int ret;

  ret = getrlimit(resource, &limit);
  printf("%-14s", name);
  if (ret == -1) {
    printf("(can't determine)\n");
  } else {
    printf("soft limit =%ld, hard limit =%ld\n", (long)limit.rlim_cur,
           (long)limit.rlim_max);
  }
}