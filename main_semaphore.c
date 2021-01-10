#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/semaphores.h"

#define N 5
int S = 0;
mutex_queue mtx;
semaphore sem;

void init() {
  S = 0;
  semaphore_init(&sem, S);
  mutex_queue_init(&mtx);
}

int barrier_point() {
  mutex_queue_lock(&mtx);
  S++;
  mutex_queue_unlock(&mtx);

  if (S < N) {
    if (semaphore_wait(&sem)) {
      perror(NULL);
      return errno;
    };

  } else if (S == N) {
    for (int i = 0; i < N; i++) {
      if (semaphore_post(&sem)) {
        perror(NULL);
        return errno;
      };
    }
  }
  return 0;
}

void* tfun(void* v) {
  int* tid = (int*)v;
  printf("%d reached the barrier\n", *tid);
  barrier_point();
  printf("%d passed the barrier\n", *tid);

  free(tid);
  return NULL;
}

int main() {
  // Initializez semaforul
  init();
  // Creez si pornesc threadurile
  pthread_t* thr = malloc(sizeof(pthread_t) * N);
  for (int i = 0; i < N; i++) {
    int* tid = malloc(sizeof(int));
    *tid = i;
    if (pthread_create(&thr[i], NULL, tfun, tid)) {
      perror(NULL);
      return errno;
    }
  }

  // Join la threaduri
  for (int i = 0; i < N; i++) {
    if (pthread_join(thr[i], NULL)) {
      perror(NULL);
      return errno;
    }
  }
  printf("Operatiune efectuata cu succes\n");
  free(thr);
  mutex_queue_destroy(&mtx);
  semaphore_destroy(&sem);

  return 0;
}