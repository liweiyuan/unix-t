#ifndef __IPC_COMMON_H__
#define __IPC_COMMON_H__

#include <pthread.h>

#define SHM_NAME "/demo_shm"
#define SEM_AVAIL "/sem_avail"
#define SEM_DONE "/sem_done"
#define SHM_SIZE 4096

struct shm_header {
  pthread_mutex_t mutex;
  size_t capacity;
  size_t data_size;
  volatile int ready_flag;
  char data[];
};

#endif