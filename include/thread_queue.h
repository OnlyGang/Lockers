#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#define HASH_SIZE 100003

typedef struct tq_node {
  pthread_t *thr;
  struct tq_node *next;
  int hash_id;
} tq_node;

typedef struct thread_queue {
  int count;
  int new_id;
  bool *in_queue;
  tq_node *front;
  tq_node *back;
} thread_queue;

void queue_init(thread_queue *q);

void queue_remove(thread_queue *q);

tq_node *create_node(pthread_t *thr);

void free_node(tq_node *node);

int push(thread_queue *q, pthread_t *thr);

pthread_t *pop(thread_queue *q);

pthread_t *front(thread_queue *q);

int size(thread_queue *q);

int empty(thread_queue *q);

void queue_test(thread_queue *q);