#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

sem_t *semph;

int main() {
	int numWakeup = 0;

	semph = sem_open("semph_name", 0);
	printf("Thread-waker PID = %d\n", getpid());

	do {
		printf("Enter the number of threads to wake-up: ");
		cin >> numWakeup;

		for (int i = 0; i < numWakeup; i++) {
			sem_post(semph);
		}

	} while (numWakeup != 0);
	sem_close(semph);
	return EXIT_SUCCESS;
}
