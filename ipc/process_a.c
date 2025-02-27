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

int main() {

  // Create a shared memory segment
  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }
  // Set the size of the shared memory segment
  if (ftruncate(shm_fd, SHM_SIZE) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }
  // Map the shared memory segment into the process's address space
  struct shm_header *header =
      mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (header == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
  // 初始化互斥锁
  // 为了确保共享内存在多个进程之间访问时不会产生竞态条件，需要用互斥锁进行同步
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr); // 初始化互斥锁属性
  pthread_mutexattr_setpshared(
      &attr, PTHREAD_PROCESS_SHARED); // 设置互斥锁在进程间共享

  // 初始化互斥锁
  if (pthread_mutex_init(&header->mutex, &attr) != 0) {
    perror("pthread_mutex_init");
    exit(EXIT_FAILURE);
  }
  // 初始化共享内存元数据
  header->capacity = SHM_SIZE; // 设置共享内存初始量
  header->data_size = 0;       // 设置数据大小
  header->ready_flag = 0;      // 设置数据就绪标志为 0，表示没有数据准备好

  // 创建信号量
  // 信号量用于进程之间同步，确保生产者和消费者之间的正确执行顺序。
  sem_unlink(SEM_AVAIL); // 删除可能已经存在的信号量
  sem_unlink(SEM_DONE);  // 删除可能已经存在的信号量

  // 创建信号量
  sem_t *avail = sem_open(SEM_AVAIL, O_CREAT | O_EXCL, 0666, 0);
  if (avail == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
  sem_t *done = sem_open(SEM_DONE, O_CREAT | O_EXCL, 0666, 0);
  if (done == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  // 测试数据
  const char *messages[] = {"Hello", "World", "Dynamic", "Memory"};
  // 循环发送
  for (int i = 0; i < 4; i++) {

    size_t len = strlen(messages[i]) + 1; // 计算消息长度
    // 加锁
    pthread_mutex_lock(&header->mutex);
    // 检查是否有足够的空间
    if (len > (header->capacity - sizeof(struct shm_header))) {
      // 扩容
      size_t new_capacity = header->capacity * 2;
      while (new_capacity < (len + sizeof(struct shm_header))) {
        new_capacity *= 2;
      }
      // 接触内存映射
      if (munmap(header, header->capacity) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
      }
      // 调整共享内存大小
      if (ftruncate(shm_fd, new_capacity) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
      }
      // 重新映射共享内存
      header = mmap(NULL, new_capacity, PROT_READ | PROT_WRITE, MAP_SHARED,
                    shm_fd, 0);
      if (header == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
      }
      // 更新共享内存容量
      header->capacity = new_capacity;
    }

    // 复制消息到共享内存
    // 写入数据到共享内存
    memcpy(header->data, messages[i], len); // 将消息复制到共享内存
    header->data_size = len;                // 更新数据的大小
    header->ready_flag = 1;                 // 设置数据已准备好的标志
    // 解锁
    pthread_mutex_unlock(&header->mutex);
    // 打印数据
    printf("[A] Send: %s\n", messages[i]);
    // 发送信号量
    sem_post(avail); // 发送信号量，表示有数据可用

    // 等待信号量
    sem_wait(done); // 等待信号量，表示数据已被处理
  }

  // 清除资源
  munmap(header, SHM_SIZE);
  close(shm_fd);
  sem_close(avail);
  sem_close(done);
  sem_unlink(SEM_AVAIL);
  sem_unlink(SEM_DONE);
  return 0;
}