# Lockers

This is the project for our Operating Systems (OS) course. It's an implementation of some clasic lockers for threads: Mutex Locks, RW Locks and Semaphores.

The books and articles that were used and studied are located in the [Resources](Resources) folder.

To check for memory leaks, use:

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main
```

Questions:

1. Do we need pthread_mutex_atrr?
2. Ask about implementation (if good/wrong)
3. pthread_cond_wait (ask about condition variables)
4. block and wakeup?
