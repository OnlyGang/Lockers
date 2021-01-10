#define _XOPEN_SOURCE 500

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "include/mutex_queue.h"

#define MAX_RESOURCES 5
#define N 5
int available_resources = MAX_RESOURCES;

// Mutexul
mutex_queue mtx;

int decrease_count(int count) {
  mutex_queue_lock(&mtx);

  if (available_resources < count) {
    mutex_queue_unlock(&mtx);
    return -1;
  } else {
    available_resources -= count;
    printf("Got %d resources, %d remaining\n", count, available_resources);
  }

  mutex_queue_unlock(&mtx);
  return 0;
}

int increase_count(int count) {
  mutex_queue_lock(&mtx);
  available_resources += count;
  printf("Released %d resources, %d remaining\n", count, available_resources);
  mutex_queue_unlock(&mtx);
  return 0;
}

// Functia apelata de thread-uri
void* pthread_function(void* v) {
  int* req = (int*)v;
  if (decrease_count(*req) == -1) {
    printf("Eroare la alocarea de %d resurse\n", *req);
  } else {
    increase_count(*req);
  }
  free(req);
  return NULL;
}

int main() {
  // Initializez mutex-ul
  mutex_queue_init(&mtx);

  bool ok = true;
  // Thread-urile (crearea lor)
  pthread_t* thr = malloc(sizeof(pthread_t) * N);
  int* res = malloc(sizeof(int) * N);

  // Cate resurse va avea nevoie fiecare
  for (int i = 0; i < N; i++) {
    res[i] = 1 + i / 2;
  }

  for (int i = 0; i < 5; i++) {
    int* req = malloc(sizeof(int));
    *req = res[i];
    if (pthread_create(&thr[i], NULL, pthread_function, req)) {
      perror(NULL);
      return errno;
    }
  }

  // Join threads (pentru main program)
  for (int i = 0; i < 5; i++) {
    if (pthread_join(thr[i], NULL)) {
      perror(NULL);
      return errno;
    }
  }

  ok &= (available_resources == MAX_RESOURCES);
  free(thr);
  free(res);

  if (ok)
    printf("Operatiune efectuata cu succes\n");
  else
    printf("Eroare!");

  mutex_queue_destroy(&mtx);

  return 0;
}