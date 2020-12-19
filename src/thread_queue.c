#include "../include/thread_queue.h"

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void queue_init(thread_queue *q) {
  q->count = 0;
  q->front = NULL;
  q->back = NULL;
}

void queue_remove(thread_queue *q) {
  tq_node *head = q->front;
  while (head != NULL) {
    tq_node *nx = head->next;
    free(head);
    head = nx;
  }

  free(q);
}

tq_node *create_node(pthread_t *thr) {
  tq_node *new_node = malloc(sizeof(tq_node));
  new_node->thr = thr;
  new_node->next = NULL;
  return new_node;
}

void free_node(tq_node *node) { free(node); }

void push(thread_queue *q, pthread_t *thr) {
  tq_node *new_node = create_node(thr);
  if (q->back == NULL) {
    q->front = new_node;
    q->back = new_node;
  } else {
    q->back->next = new_node;
    q->back = new_node;
  }
  q->count++;
}

pthread_t *front(thread_queue *q) {
  return (q->front != NULL ? q->front->thr : NULL);
}

pthread_t *pop(thread_queue *q) {
  if (q->count == 0) {
    return NULL;
  }
  tq_node *to_remove = q->front;
  pthread_t *popped = to_remove->thr;
  q->front = to_remove->next;
  if (q->count == 1) q->back = NULL;
  free_node(to_remove);
  q->count--;
  return popped;
}

int size(thread_queue *q) { return q->count; }

int empty(thread_queue *q) { return (q->count == 0); }

void queue_test(thread_queue *q) {
  tq_node *it = q->front;
  while (it != NULL) {
    printf("%ld ", *(it->thr));
    it = it->next;
  }
  printf("\n");
}