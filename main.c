#include <stdio.h>

#include "include/thread_queue.h"

int main() {
  printf("%s", "Merge\n");

  thread_queue *q = malloc(sizeof(thread_queue));
  queue_init(q);

  pthread_t *thr = malloc(6 * sizeof(pthread_t));
  for (int i = 0; i < 6; i++) {
    thr[i] = i;
    push(q, thr + i);
  }

  queue_test(q);

  printf("%ld\n", *pop(q));
  queue_test(q);

  printf("%ld\n", *front(q));
  queue_test(q);

  while (!empty(q)) {
    printf("Size: %d ", size(q));
    printf("pop: %ld\n", *front(q));
    pop(q);
    queue_test(q);
  }

  free(thr);
  queue_remove(q);

  return 0;
}