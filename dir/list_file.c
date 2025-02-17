#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void list_files(const char *path, int depth) {
  DIR *dir;
  struct dirent *entry;
  struct stat file_stat;
  char new_path[1024];

  // 打开目录
  dir = opendir(path);
  if (dir == NULL) {
    perror("opendir");
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    // 跳过. 和..
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    // 构建新路径
    snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);

    // 获取文件或目录的状态信息
    if (stat(new_path, &file_stat) == -1) {
      perror("stat");
      continue;
    }

    // 打印缩进，用于表示目录层次
    for (int i = 0; i < depth; i++) {
      printf("  ");
    }

    if (S_ISDIR(file_stat.st_mode)) {
      printf("Dir: %s\n", entry->d_name);
      // 递归处理子目录
      list_files(new_path, depth + 1);
    } else {
      printf("File: %s\n", entry->d_name);
    }
  }

  // 关闭目录
  closedir(dir);
}

int main() {
  const char *directory = ".";
  list_files(directory, 0);
  return 0;
}