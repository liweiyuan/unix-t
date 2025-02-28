#include "ipc_common.h"
#include <bits/pthreadtypes.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// 前向声明新抽象的函数
static sem_t *open_semaphore(const char *name);
static void remap_shared_memory(int shm_fd, struct shm_header **header);
static void process_data(struct shm_header *header);
static void cleanup_resources(struct shm_header *header, int shm_fd,
                              sem_t *sem_avail, sem_t *sem_done);

int main() {
  // 打开共享内存对象（由process_a创建）
  // O_RDWR: 以读写模式打开，0666: 设置读写权限
  int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
  if (shm_fd == -1) {
    perror("shm_open failed");
    exit(EXIT_FAILURE);
  }

  // 获取共享内存状态信息
  // 包含文件大小、权限、时间戳等元数据
  struct stat shm_stat;
  if (fstat(shm_fd, &shm_stat) == -1) {
    perror("fstat failed");
    exit(EXIT_FAILURE);
  }

  // 内存映射操作（建立进程虚拟地址到共享内存的映射）
  // MAP_SHARED: 修改会同步到文件，PROT_READ|PROT_WRITE: 可读写
  struct shm_header *header = mmap(
      NULL, shm_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (header == MAP_FAILED) {
    perror("Initial mmap failed");
    exit(EXIT_FAILURE);
  }

  // 打开两个POSIX信号量（用于进程间同步）
  // SEM_AVAIL: 数据可用信号量，SEM_DONE: 数据处理完成信号量
  sem_t *sem_avail = open_semaphore(SEM_AVAIL);
  sem_t *sem_done = open_semaphore(SEM_DONE);

  // 主处理循环（固定4次迭代，对应process_a的4次发送）
  for (int i = 0; i < 4; i++) {
    // 等待数据可用信号量（阻塞直到process_a发布数据）
    sem_wait(sem_avail);

    // 获取共享内存的互斥锁（防止并发访问冲突）
    pthread_mutex_lock(&header->mutex);

    // 动态内存管理：检查共享内存是否扩容
    // 使用双指针传递header以支持内存地址更新
    remap_shared_memory(shm_fd, &header);

    // 核心数据处理逻辑
    process_data(header);

    // 释放互斥锁（允许其他进程访问共享内存）
    pthread_mutex_unlock(&header->mutex);

    // 发送数据处理完成信号（通知process_a可以发送新数据）
    sem_post(sem_done);
  }

  // 统一清理所有系统资源
  cleanup_resources(header, shm_fd, sem_avail, sem_done);
  return 0;
}

// 新抽象的函数实现
static sem_t *open_semaphore(const char *name) {
  // 打开已存在的命名信号量
  // 第二个参数0表示只打开不创建新信号量
  sem_t *sem = sem_open(name, 0);
  if (sem == SEM_FAILED) {
    perror("sem_open failed");
    exit(EXIT_FAILURE);
  }
  return sem;
}

static void remap_shared_memory(int shm_fd, struct shm_header **header) {
  // 重新获取共享内存的最新状态
  struct stat shm_stat;
  if (fstat(shm_fd, &shm_stat) == -1) {
    perror("fstat failed");
    exit(EXIT_FAILURE);
  }

  // 检查共享内存是否扩容（通过比较当前尺寸和记录的容量）
  if (shm_stat.st_size > (*header)->capacity) {
    // 解除旧内存映射（使用原始容量值）
    munmap(*header, (*header)->capacity);

    // 重新建立内存映射（使用新的文件尺寸）
    // 新映射会继承之前的MAP_SHARED属性
    *header = mmap(NULL, shm_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED,
                   shm_fd, 0);
    if (*header == MAP_FAILED) {
      perror("Remap mmap failed");
      exit(EXIT_FAILURE);
    }
  }
}

static void process_data(struct shm_header *header) {
  // 检查数据就绪标志（由process_a设置）
  if (header->ready_flag) {
    // 使用临时缓冲区保证数据完整性
    char buf[256];

    // 安全拷贝数据（避免直接操作共享内存）
    memcpy(buf, header->data, header->data_size);

    // 重置标志位（采用置零方式保证原子性）
    header->ready_flag = 0;

    // 输出带进程标识的信息（方便调试观察）
    printf("[B] Received: %s\n", buf);
  }
}

static void cleanup_resources(struct shm_header *header, int shm_fd,
                              sem_t *sem_avail, sem_t *sem_done) {
  // 解除内存映射（使用当前记录的容量值）
  munmap(header, header->capacity);

  // 关闭共享内存文件描述符（不会删除共享内存对象）
  close(shm_fd);

  // 关闭信号量连接（不会删除命名信号量）
  sem_close(sem_avail);
  sem_close(sem_done);
}