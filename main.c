#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "include/semaphores.h"

#define N 5
int S = 0;
pthread_mutex_t mtx;
semaphore sem;

void init() { semaphore_init(&sem, 0); }

int barrier_point() {
  pthread_mutex_lock(&mtx);
  S++;
  pthread_mutex_unlock(&mtx);

  if (S < N) {
    if (semaphore_wait(&sem)) {
      perror(NULL);
      return errno;
    };

  } else if (S == N) {
    queue_test(sem.q);
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
  printf("%ld reached the barrier\n", pthread_self());
  barrier_point();
  printf("%ld passed the barrier\n", pthread_self());

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

  // // Join la threaduri
  for (int i = 0; i < N; i++) {
    if (pthread_join(thr[i], NULL)) {
      perror(NULL);
      return errno;
    }
  }

  return 0;
}
