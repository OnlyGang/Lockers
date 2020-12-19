#include "../include/atomic_functions.h"

#include <stdbool.h>
#include <stdlib.h>

int compare_and_swap(int *value, int expected, int new_value) {
  int temp = *value;
  if (*value == expected) *value = new_value;
  return temp;
}

bool test_and_set(bool *target) {
  bool rv = *target;
  *target = true;
  return rv;
}
