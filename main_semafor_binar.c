#define _XOPEN_SOURCE 500

#include <errno.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "include/semaphores.h"

#define MAX_RESOURCES 5
#define N 5
int available_resources = MAX_RESOURCES;

// Mutexul
semaphore mtx;

int decrease_count(int count) {
  semaphore_wait(&mtx);

  if (available_resources < count) {
    semaphore_post(&mtx);
    return -1;
  } else {
    available_resources -= count;
    printf("Got %d resources, %d remaining\n", count, available_resources);
  }

  semaphore_post(&mtx);
  return 0;
}

int increase_count(int count) {
  semaphore_wait(&mtx);
  available_resources += count;
  printf("Released %d resources, %d remaining\n", count, available_resources);
  semaphore_post(&mtx);
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
  semaphore_init(&mtx, 1);

  bool ok = true;
  int tests = 1;
  while (tests--) {
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
  }
  printf("Tests passed %d \n", ok);

  semaphore_destroy(&mtx);

  return 0;
}