#define _XOPEN_SOURCE 500

#include <errno.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
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
  int tests = 10000;
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

  mutex_queue_destroy(&mtx);

  return 0;
}

// void signal_handler(int sig) {
//   printf("Caught signal %d\n", sig);
//   signal(SIGUSR1, signal_handler);
// }

// void* fun1(void* v) {
//   signal(SIGUSR1, signal_handler);
//   // rest of your code here
//   int* pi = (int*)v;
//   printf("Pause %d\n", *pi);
//   if (*pi != 4) pause();
//   printf("Woken %d\n", *pi);
//   return v;
// }

// void* fun2(void* v) {
//   pthread_t* thr = (pthread_t*)v;
//   printf("Noapte buna\n");
//   sleep(2);
//   printf("Neata\n");
//   printf("Incep sa-i trezesec\n");
//   for (int i = 0; i < 5; i++) {
//     printf("trezesc pe %d: ", i);
//     pthread_kill(thr[i], SIGUSR1);
//     printf("trezit %d\n", i);
//     usleep((useconds_t)1000);
//   }

//   return NULL;
// }

// int main() {
//   // Creez si pornesc threadurile
//   pthread_t* thr = malloc(sizeof(pthread_t) * 7);

//   for (int i = 0; i < 5; i++) {
//     int* pi = malloc(sizeof(int));
//     *pi = i;
//     pthread_create(&thr[i], NULL, fun1, pi);
//   }

//   sleep(5);

//   pthread_create(&thr[5], NULL, fun2, thr);
//   // sleep(5);
//   // pthread_create(&thr[6], NULL, fun2, &thr[3]);

//   // printf("Woken all up\n");

//   pthread_join(thr[5], NULL);
//   // pthread_join(thr[6], NULL);
//   // pthread_kill(thr[0], SIGCONT);
//   for (int i = 0; i < 5; i++) pthread_join(thr[i], NULL);

//   printf("atomic part:\n");
//   // atomic_flag test = ATOMIC_FLAG_INIT;
//   atomic_bool val = 0;
//   // int ans2 = atomic_flag_test_and_set(&test);
//   int thing = __atomic_test_and_set(&val, __ATOMIC_ACQ_REL);
//   // atomic_load(&test);
//   // int ans = atomic_compare_exchange_strong(&test, &val, val);
//   printf("value: %d %d\n", thing, atomic_load(&val));

//   return 0;
// }

// // cas(mutex->lock, true, false)

// // cas(mutex->lock, true , mutex->lock)

// /*
//   tas(true) -> true -> set to true
//   tas(false) -> false -> set to true

//   cas(x, y, z) -> returneaza true si face x = true
//   cas(a, b, c) -> returneaza false si face a = true
// */