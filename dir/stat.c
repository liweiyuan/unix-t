#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  // 创建文件
  FILE *file = fopen("test.txt", "w");

  struct stat file_stat; // 获取文件状态
  if (stat("test.txt", &file_stat) < 0) {
    perror("stat");
    return 1;
  }
  printf("文件大小: %ld\n", file_stat.st_size);
  printf("文件权限: %o\n", file_stat.st_mode & 0777);
  printf("文件类型: %d\n", file_stat.st_mode & S_IFMT);
  // 输出文件的inode
  printf("文件inode: %ld\n", file_stat.st_ino);

  fclose(file);

  return 0;
}