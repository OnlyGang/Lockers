#include "../include/rwlocks.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void rw_lock_init(rw_lock *rw) {
  semaphore_init(&rw->lock, 1);
  semaphore_init(&rw->writelock, 1);
}

int rw_lock_acquire_readlock(rw_lock *rw) {
  semaphore_wait(&rw->lock);
  rw->readers++;
  if (rw->readers == 1) {
    semaphore_wait(&rw->lock);
  }
  semaphore_post(&rw->lock);
  return 0;
}

int rw_lock_release_readlock(rw_lock *rw) {
  semaphore_wait(&rw->lock);
  rw->readers--;
  if (rw->readers == 0) {
    semaphore_post(&rw->writelock);
  }
  semaphore_post(&rw->lock);
  return 0;
}

int rw_lock_acquire_writelock(rw_lock *rw) {
  return semaphore_wait(&rw->writelock);
}

int rw_lock_release_writelock(rw_lock *rw) {
  return semaphore_wait(&rw->writelock);
}

void rw_lock_destroy(rw_lock *rw) {
  semaphore_destroy(&rw->lock);
  semaphore_destroy(&rw->writelock);
}