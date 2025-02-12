#include <stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) {
    printf(" %.2x", start[i]);
  }
  printf("\n");
}

// int指针
void show_int(int x) {
  printf("x = %d", x);
  show_bytes((byte_pointer)&x, sizeof(x));
}

void show_float(float x) {
  printf("float = %f", x);
  show_bytes((byte_pointer)&x, sizeof(x));
}

void show_pointer(void *x) {
  int *intPtr = (int *)x;
  printf("point = %d", *intPtr);
  show_bytes((byte_pointer)&x, sizeof(x));
}

void test_show_bytes(int val) {
  int ival = val;
  float fval = (float)ival;
  int *pval = &ival;
  show_int(ival);
  show_float(fval);
  show_pointer(pval);
}

int main() {
  test_show_bytes(12345);
  return 0;
}