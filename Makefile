main_mutex: main_mutex.o thread_queue.o atomic_functions.o mutex_sleep.o mutex_queue.o rwlocks.o semaphores.o
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g main_mutex.o thread_queue.o atomic_functions.o mutex_sleep.o mutex_queue.o rwlocks.o semaphores.o -o main_mutex -pthread

main_semafor_binar: main_semafor_binar.o thread_queue.o atomic_functions.o mutex_sleep.o mutex_queue.o rwlocks.o semaphores.o
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g main_semafor_binar.o thread_queue.o atomic_functions.o mutex_sleep.o mutex_queue.o rwlocks.o semaphores.o -o main_semafor_binar -pthread

main_semaphore: main_semaphore.o thread_queue.o atomic_functions.o mutex_sleep.o mutex_queue.o rwlocks.o semaphores.o
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g main_semaphore.o thread_queue.o atomic_functions.o mutex_sleep.o mutex_queue.o rwlocks.o semaphores.o -o main_semaphore -pthread

main_rwlocks: main_rwlocks.o thread_queue.o atomic_functions.o mutex_sleep.o mutex_queue.o rwlocks.o semaphores.o
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g main_rwlocks.o thread_queue.o atomic_functions.o mutex_sleep.o mutex_queue.o rwlocks.o semaphores.o -o main_rwlocks -pthread

main_mutex.o: main_mutex.c
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c main_mutex.c -pthread

main_semaphore.o: main_semaphore.c
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c main_semaphore.c -pthread

main_rwlocks.o: main_rwlocks.c
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c main_rwlocks.c -pthread

thread_queue.o: src/thread_queue.c include/thread_queue.h
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c src/thread_queue.c -pthread

atomic_functions.o: src/atomic_functions.c include/atomic_functions.h
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c src/atomic_functions.c -pthread

mutex_sleep.o: src/mutex_sleep.c include/mutex_sleep.h
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c src/mutex_sleep.c -pthread

mutex_queue.o: src/mutex_queue.c include/mutex_queue.h
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c src/mutex_queue.c -pthread

rwlocks.o: src/rwlocks.c include/rwlocks.h
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c src/rwlocks.c -pthread

semaphores.o: src/semaphores.c include/semaphores.h
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c src/semaphores.c -pthread

cleano:
	rm *.o

clean:
	rm *.o main_mutex main_semaphore main_rwlocks