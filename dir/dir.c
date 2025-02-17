#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  // 创建目录
  if (mkdir("test_dir", 0755) < 0) {
    perror("mkdir");
    return 1;
  }

  // 创建符号链接
  if (symlink("test_dir", "test_link") < 0) {
    perror("symlink");
    return 1;
  }

  // 删除符号链接
  if (unlink("test_link") < 0) {
    perror("unlink");
    return 1;
  }

  // 删除目录
  if (rmdir("test_dir") < 0) {
    perror("rmdir");
    return 1;
  }

  DIR *dp = opendir(".");
  if (dp == NULL) {
    perror("opendir");
    return 1;
  }
  struct dirent *entry;
  // 读取目录内容
  while ((entry = readdir(dp)) != NULL) {
    printf("%s\n", entry->d_name);
  }
  closedir(dp);

  return 0;
}