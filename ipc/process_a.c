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

// 前向声明抽象化函数
static void init_shared_memory(int *shm_fd, struct shm_header **header);
static void init_mutex(struct shm_header *header);
static void init_semaphores(sem_t **avail, sem_t **done);
static void resize_shared_memory(int shm_fd, struct shm_header **header,
                                 size_t new_capacity);
static void send_messages(struct shm_header *header, int shm_fd, sem_t *avail,
                          sem_t *done);
static void cleanup_resources(struct shm_header *header, int shm_fd,
                              sem_t *avail, sem_t *done);

int main() {
  int shm_fd;
  struct shm_header *header;
  sem_t *avail, *done;

  init_shared_memory(&shm_fd, &header);
  init_mutex(header);
  init_semaphores(&avail, &done);

  // 主消息发送流程
  send_messages(header, shm_fd, avail, done);

  cleanup_resources(header, shm_fd, avail, done);
  return 0;
}

/* 初始化共享内存区域（包含元数据头） */
static void init_shared_memory(int *shm_fd, struct shm_header **header) {
  // 创建或打开共享内存对象（O_CREAT: 不存在则创建）
  *shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  if (*shm_fd == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  // 设置共享内存初始大小（包含头部元数据）
  if (ftruncate(*shm_fd, SHM_SIZE) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }

  // 内存映射：将共享内存映射到进程地址空间
  *header =
      mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, *shm_fd, 0);
  if (*header == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  // 初始化共享内存头部信息
  (*header)->capacity = SHM_SIZE;
  (*header)->ready_flag = 0;
}

/* 初始化进程间互斥锁（设置跨进程共享属性） */
static void init_mutex(struct shm_header *header) {
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  // 设置跨进程共享属性（关键参数）
  pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

  if (pthread_mutex_init(&header->mutex, &attr) != 0) {
    perror("pthread_mutex_init");
    exit(EXIT_FAILURE);
  }
}

/* 创建并初始化POSIX信号量 */
static void init_semaphores(sem_t **avail, sem_t **done) {
  // 清理可能存在的残留信号量
  sem_unlink(SEM_AVAIL);
  sem_unlink(SEM_DONE);

  // 创建互斥信号量（O_EXCL: 确保创建新信号量）
  *avail = sem_open(SEM_AVAIL, O_CREAT | O_EXCL, 0666, 0);
  if (*avail == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  *done = sem_open(SEM_DONE, O_CREAT | O_EXCL, 0666, 0);
  if (*done == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
}

/* 动态扩展共享内存容量（使用加倍策略） */
static void resize_shared_memory(int shm_fd, struct shm_header **header,
                                 size_t new_capacity) {
  // 解除旧内存映射（必须使用原始容量值）
  if (munmap(*header, (*header)->capacity) == -1) {
    perror("munmap");
    exit(EXIT_FAILURE);
  }

  // 调整共享内存文件大小
  if (ftruncate(shm_fd, new_capacity) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }

  // 重新建立内存映射
  *header =
      mmap(NULL, new_capacity, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (*header == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  // 更新容量记录（供process_b检测使用）
  (*header)->capacity = new_capacity;
}

/* 消息发送主逻辑（带自动扩容机制） */
static void send_messages(struct shm_header *header, int shm_fd, sem_t *avail,
                          sem_t *done) {
  const char *messages[] = {"Hello", "World", "Dynamic", "Memory"};

  for (int i = 0; i < 4; i++) {
    const char *msg = messages[i];
    size_t required_size = sizeof(struct shm_header) + strlen(msg) + 1;

    pthread_mutex_lock(&header->mutex);

    // 动态扩容检查（使用加倍策略避免频繁扩容）
    if (required_size > header->capacity) {
      size_t new_capacity = header->capacity * 2;
      while (new_capacity < required_size) {
        new_capacity *= 2;
      }
      resize_shared_memory(shm_fd, &header, new_capacity);
    }

    // 安全写入数据
    memcpy(header->data, msg, strlen(msg) + 1);
    header->data_size = strlen(msg) + 1;
    header->ready_flag = 1;

    pthread_mutex_unlock(&header->mutex);

    // 同步机制：通知-等待循环
    sem_post(avail); // 通知process_b数据就绪
    sem_wait(done);  // 等待process_b处理完成
    printf("[A] Send: %s\n", msg);
  }
}

/* 统一资源清理操作 */
static void cleanup_resources(struct shm_header *header, int shm_fd,
                              sem_t *avail, sem_t *done) {
  // 注意清理顺序：先解除映射再关闭描述符
  munmap(header, header->capacity);
  close(shm_fd);

  // 关闭并删除信号量（生产者负责清理）
  sem_close(avail);
  sem_close(done);
  sem_unlink(SEM_AVAIL);
  sem_unlink(SEM_DONE);
}