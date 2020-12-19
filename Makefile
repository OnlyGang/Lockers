main: main.o thread_queue.o atomic_functions.o mutex_atomic.o rwlocks.o semaphores.o
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g main.o thread_queue.o atomic_functions.o mutex_atomic.o rwlocks.o semaphores.o -o main -pthread

main.o: main.c
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c main.c -pthread

thread_queue.o: src/thread_queue.c include/thread_queue.h
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c src/thread_queue.c -pthread

atomic_functions.o: src/atomic_functions.c include/atomic_functions.h
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c src/atomic_functions.c -pthread

mutex_atomic.o: src/mutex_atomic.c include/mutex_atomic.h
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c src/mutex_atomic.c -pthread

mutex_queue.o: src/mutex_queue.c include/mutex_queue.h
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c src/mutex_queue.c -pthread

rwlocks.o: src/rwlocks.c include/rwlocks.h
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c src/rwlocks.c -pthread

semaphores.o: src/semaphores.c include/semaphores.h
	gcc -std=c11 -Wall -Wextra -Werror -Wfatal-errors -g -c src/semaphores.c -pthread

cleano:
	rm *.o

clean:
	rm *.o main