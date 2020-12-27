#define _XOPEN_SOURCE 500

#include "../include/semaphores.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void signal_handler_2(int sig) {
  (void)sig;
  // printf("Caught signal %d\n", sig);
  signal(SIGUSR2, signal_handler_2);
}

void semaphore_init(semaphore* sem, int val) {  // change to int
  sem->S = val;
  sem->mtxq = malloc(sizeof(mutex_queue));
  mutex_queue_init(sem->mtxq);
  sem->sem_q = malloc(sizeof(thread_queue));
  queue_init(sem->sem_q);
  signal(SIGUSR2, signal_handler_2);
  // pthread_mutex_init(&sem->mtx, NULL);
}

int semaphore_wait(semaphore* sem) {
  // usleep((useconds_t)100);
  mutex_queue_lock(sem->mtxq);
  // pthread_mutex_lock(&sem->mtx);
  // printf("Thread %ld entered wait\n", pthread_self() % 1000);
  pthread_t* thr = malloc(sizeof(pthread_t));
  *thr = pthread_self();
  if (sem->S > 0) {  // daca mai avem locuri libere
    sem->S--;
  } else {  // daca nu, bagam in coada si stopam
    // printf("Thread %ld entered queue\n", pthread_self() % 1000);
    int hash_id = push(sem->sem_q, thr);
    mutex_queue_unlock(sem->mtxq);
    // pthread_mutex_unlock(&sem->mtx);
    // printf("Thread %ld left mutex\n", pthread_self() % 1000);
    if (sem->sem_q->in_queue[hash_id] == true) pause();
    mutex_queue_lock(sem->mtxq);
    // pthread_mutex_lock(&sem->mtx);
    sem->S--;  // dupa ce s-a reluat executia thr, semnalam ca s-a ocupat un loc
               // (a avut loc post unde S++)
  }

  // pthread_mutex_unlock(&sem->mtx);
  mutex_queue_unlock(sem->mtxq);
  return 0;
}

int semaphore_post(semaphore* sem) {
  mutex_queue_lock(sem->mtxq);
  // pthread_mutex_lock(&sem->mtx);

  sem->S++;  // indiferent de starea cozii, semnalam ca s-a eliberat un loc
  // daca inainte de semnalare nu erau locuri, trebuie sa
  // verificam daca cineva astepta

  if (!empty(sem->sem_q)) {  // daca exista procese care asteptau
    usleep((useconds_t)1000);
    pthread_t* next = pop(sem->sem_q);
    if (pthread_kill(*next, SIGUSR2) != 0) {
      perror("Couldn't wake up a thread\n");
      return errno;
    };
    // printf("Waiting\n");
    usleep((useconds_t)100);
    free(next);
  }
  mutex_queue_unlock(sem->mtxq);
  // pthread_mutex_unlock(&sem->mtx);
  return 0;
}

void semaphore_destroy(semaphore* sem) {
  mutex_queue_destroy(sem->mtxq);
  // pthread_mutex_destroy(&sem->mtx);
  free(sem->mtxq);
  queue_remove(sem->sem_q);
}