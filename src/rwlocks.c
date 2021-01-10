#include "../include/rwlocks.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void rw_lock_init(rw_lock *rw) {
  mutex_busy_init(&rw->lock);
  mutex_busy_init(&rw->writelock);
}

int rw_lock_acquire_readlock(rw_lock *rw) {
  mutex_busy_lock(&rw->lock);
  rw->readers++;
  if (rw->readers == 1) {
    mutex_busy_lock(&rw->writelock);
  }
  mutex_busy_unlock(&rw->lock);
  return 0;
}

int rw_lock_release_readlock(rw_lock *rw) {
  mutex_busy_lock(&rw->lock);
  rw->readers--;
  if (rw->readers == 0) {
    mutex_busy_unlock(&rw->writelock);
  }
  mutex_busy_unlock(&rw->lock);
  return 0;
}

int rw_lock_acquire_writelock(rw_lock *rw) {
  return mutex_busy_lock(&rw->writelock);
}

int rw_lock_release_writelock(rw_lock *rw) {
  return mutex_busy_unlock(&rw->writelock);
}

void rw_lock_destroy(rw_lock *rw) {
  mutex_busy_destroy(&rw->lock);
  mutex_busy_destroy(&rw->writelock);
}