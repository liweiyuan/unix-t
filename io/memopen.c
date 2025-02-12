#include <apue.h>
#include <stdio.h>

int main() {
  char buffer[] = "Hello, World!";     // 初始化内存缓冲区
  size_t buffer_size = strlen(buffer); // 获取缓冲区大小

  // 打开内存流
  FILE *stream = fmemopen(buffer, buffer_size, "r+");
  if (stream == NULL) {
    printf("Failed to open memory stream.\n");
    return 1;
  }

  char read_buffer[16]; // 用于存储读取的数据
  size_t bytes_read = fread(read_buffer, 1, sizeof(read_buffer), stream);
  read_buffer[bytes_read] = '\0'; // 添加字符串结尾符
  printf("Read from memory stream: %s\n", read_buffer);

  // 写入内存流
  fseek(stream, 0, SEEK_SET); // 将流指针移动到开头
  fputs("Hello, Memory!", stream);

  // 重新读取内存流
  fseek(stream, 0, SEEK_SET);
  bytes_read = fread(read_buffer, 1, sizeof(read_buffer), stream);
  read_buffer[bytes_read] = '\0';
  fflush(stream);
  printf("Read from memory stream: %s\n", read_buffer);

  fclose(stream); // 关闭内存流
  exit(0);
}