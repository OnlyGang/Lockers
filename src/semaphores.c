#define _XOPEN_SOURCE 500

#include "../include/semaphores.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void semaphore_init(semaphore* sem, int val) {
  sem->S = val;
  sem->mtxq = malloc(sizeof(mutex_queue));
  mutex_queue_init(sem->mtxq);
  sem->sem_q = malloc(sizeof(thread_queue));
  queue_init(sem->sem_q);
}

int semaphore_wait(semaphore* sem) {
  mutex_queue_lock(sem->mtxq);
  // printf("Thread %ld entered wait\n", pthread_self() % 1000);
  pthread_t* thr = malloc(sizeof(pthread_t));
  *thr = pthread_self();
  if (sem->S > 0) {  // daca mai avem locuri libere
    sem->S--;
  } else {  // daca nu, bagam in coada si stopam
    // printf("Thread %ld entered queue\n", pthread_self() % 1000);
    int hash_id = push(sem->sem_q, thr);
    mutex_queue_unlock(sem->mtxq);
    // printf("Thread %ld left mutex\n", pthread_self() % 1000);
    while (sem->sem_q->in_queue[hash_id] == true) usleep(100);

    mutex_queue_lock(sem->mtxq);
    sem->S--;  // dupa ce s-a reluat executia thr, semnalam ca s-a ocupat un loc
               // (a avut loc post unde S++)
  }

  mutex_queue_unlock(sem->mtxq);
  return 0;
}

int semaphore_post(semaphore* sem) {
  mutex_queue_lock(sem->mtxq);
  sem->S++;  // indiferent de starea cozii, semnalam ca s-a eliberat un loc
  // daca inainte de semnalare nu erau locuri, trebuie sa
  // verificam daca cineva astepta

  if (!empty(sem->sem_q)) {  // daca exista procese care asteptau
    pthread_t* next = pop(sem->sem_q);
    free(next);
  }
  mutex_queue_unlock(sem->mtxq);
  return 0;
}

void semaphore_destroy(semaphore* sem) {
  mutex_queue_destroy(sem->mtxq);
  free(sem->mtxq);
  queue_remove(sem->sem_q);
}