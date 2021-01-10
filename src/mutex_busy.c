#include "../include/mutex_busy.h"

#define _GNU_SOURCE

#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

// Mutex: implementarea ticket lock-ului utilizand functia atomica
// fetch_and_add

int mutex_busy_init(mutex_busy* mtxb) {
  mtxb->turn = malloc(sizeof(atomic_llong));
  mtxb->ticket = malloc(sizeof(atomic_llong));
  *(mtxb->turn) = 0;
  *(mtxb->ticket) = 0;
  return 0;
}

int mutex_busy_lock(mutex_busy* mtxb) {
  long long my_turn = __sync_fetch_and_add(mtxb->ticket, 1);
  while (atomic_load(mtxb->turn) != my_turn) {
    if (pthread_yield()) {
      perror("Error occured while yielding the thread\n");
      return errno;
    }
  }
  return 0;
}

int mutex_busy_unlock(mutex_busy* mtxb) {
  // printf("%d turn\n", *(mtxb->turn));
  __sync_fetch_and_add(mtxb->turn, 1);
  return 0;
}

int mutex_busy_destroy(mutex_busy* mtxb) {
  free(mtxb->turn);
  free(mtxb->ticket);
  mtxb->turn = NULL;
  mtxb->ticket = NULL;
  return 0;
}