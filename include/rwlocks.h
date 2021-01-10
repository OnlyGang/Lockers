#ifndef RWLOCKS_
#define RWLOCKS_

#include "mutex_busy.h"

typedef struct rw_lock {
  mutex_busy writelock;  // Semafor pentru writer
  mutex_busy lock;
  int readers;
} rw_lock;

void rw_lock_init(rw_lock *rw);

int rw_lock_acquire_readlock(rw_lock *rw);

int rw_lock_release_readlock(rw_lock *rw);

int rw_lock_acquire_writelock(rw_lock *rw);

int rw_lock_release_writelock(rw_lock *rw);

void rw_lock_destroy(rw_lock *rw);

#endif  // RWLOCKS_