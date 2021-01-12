#define _XOPEN_SOURCE 500

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/rwlocks.h"

#define N 20

rw_lock account_lock;

int checking_balance = 200;
int saving_balance = 200;

void* get_checking(void* dummy) {
  rw_lock_acquire_readlock(&account_lock);
  int bal = checking_balance;
  rw_lock_release_readlock(&account_lock);
  printf("Cheking: %d\n", bal);
  return dummy;
}

void* get_saving(void* dummy) {
  rw_lock_acquire_readlock(&account_lock);
  int bal = saving_balance;
  rw_lock_release_readlock(&account_lock);
  printf("Saving: %d\n", bal);
  return dummy;
}

void* get_all(void* dummy) {
  rw_lock_acquire_readlock(&account_lock);
  int bal = checking_balance + saving_balance;
  printf("Cheking and Saving: %d\n", bal);
  rw_lock_release_readlock(&account_lock);

  return dummy;
}

void* transfer_saving_to_checking(void* amount) {
  int* i_amount = (int*)amount;
  rw_lock_acquire_writelock(&account_lock);
  saving_balance -= *i_amount;
  usleep(100);
  checking_balance += *i_amount;
  printf("Trasfered from saving to checking: %d\n", *i_amount);
  printf("Checking: %d, Saving: %d\n", checking_balance, saving_balance);
  rw_lock_release_writelock(&account_lock);
  return NULL;
}

int main() {
  rw_lock_init(&account_lock);

  pthread_t* thr = malloc(sizeof(pthread_t) * N);
  for (int i = 0; i < N; i++) {
    int* tid = malloc(sizeof(int));
    *tid = 10;
    if (i % 4 == 0) {
      if (pthread_create(&thr[i], NULL, get_all, NULL)) {
        perror(NULL);
        return errno;
      }
    } else {
      if (pthread_create(&thr[i], NULL, transfer_saving_to_checking, tid)) {
        perror(NULL);
        return errno;
      }
    }
  }

  // Join la threaduri
  for (int i = 0; i < N; i++) {
    if (pthread_join(thr[i], NULL)) {
      perror(NULL);
      return errno;
    }
  }
  printf("Operatiune efectuata cu succes\n");
  free(thr);

  rw_lock_destroy(&account_lock);
  return 0;
}