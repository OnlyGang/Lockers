#include "semaphores.h"

typedef struct rw_lock {
  semaphore lock;       // Semafor binar
  semaphore writelock;  // Semafor pentru writer
  int readers;
} rw_lock;

void rw_lock_init(rw_lock *rw);

int rw_lock_acquire_readlock(rw_lock *rw);

int rw_lock_release_readlock(rw_lock *rw);

int rw_lock_acquire_writelock(rw_lock *rw);

int rw_lock_release_writelock(rw_lock *rw);

void rw_lock_destroy(rw_lock *rw);