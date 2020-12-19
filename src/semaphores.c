#include "../include/semaphores.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void semaphore_init(semaphore* sem, int val) {  // change to int
  sem->S = val;
  sem->mtx = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(sem->mtx, NULL);
  sem->q = malloc(sizeof(thread_queue));
  sem->locked_by = NULL;
  queue_init(sem->q);
}

int semaphore_wait(semaphore* sem) {
  // pthread_mutex_lock(sem->mtx);
  pthread_t* thr = malloc(sizeof(pthread_t));
  *thr = pthread_self();
  if (sem->S > 0) {  // daca mai avem locuri libere
    sem->S--;
  } else {  // daca nu, bagam in coada si stopam
    push(sem->q, thr);
    pause();
    sem->S--;  // dupa ce s-a reluat executia thr, semnalam ca s-a ocupat un loc
               // (a avut loc post unde S++)
  }

  if (sem->S == 0) {  // trebuie sa inchidem zona critica
    sem->locked_by = thr;
  }
  return 0;
  // pthread_mutex_unlock(sem->mtx);
}

int semaphore_post(semaphore* sem) {
  sem->S++;  // indiferent de starea cozii, semnalam ca s-a eliberat un loc
             // daca inainte de semnalare nu erau locuri, trebuie sa
             // verificam daca cineva astepta
  if (*(sem->locked_by) != pthread_self()) {
    perror("semaphore tried to be unlocked by another thread\n");
    return -1;
  }

  free(sem->locked_by);
  sem->locked_by = NULL;

  if (!empty(sem->q)) {  // daca exista procese care asteptau
    pthread_t* next = pop(sem->q);
    if (pthread_kill(*next, SIGCONT) != 0) {
      perror("Couldn't wake up a thread\n");
      return errno;
    };
    free(next);
  }

  return 0;
}

void semaphore_destroy(semaphore* sem) {
  pthread_mutex_destroy(sem->mtx);
  queue_remove(sem->q);
  free(sem);
}