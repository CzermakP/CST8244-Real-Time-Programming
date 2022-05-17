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

volatile sig_atomic_t calledHandler = 0;
sem_t *semph;

void handler(int sig);
void* child_thread(void *args);

int main() {

	int nThreads = 0;
	pthread_attr_t attr;

	struct sigaction sa;
	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("ERROR, handler function could not intercept SIGUSR1");
		exit(1);
	}
	printf("Enter the number of threads to create: \n");
	cin >> nThreads;
	//scanf("%d", nThreads);

	//call semaphore open function here
	semph = sem_open("semph_name", O_CREAT, S_IWOTH, 0);

	for (int i = 0; i < nThreads; i++) {
		pthread_attr_init(&attr);
		pthread_create( NULL, &attr, &child_thread, NULL);
		pthread_attr_destroy(&attr);
	}
	while (!calledHandler) {

	}
	sem_close(semph);
	sem_unlink("semph_name");
	return EXIT_SUCCESS;
}

// Handler to set 'calledHandler' to 1
void handler(int sig) {
	calledHandler = 1;
}

// Thread handler (semaphore) - displays output to console
void* child_thread(void *args) {
	printf("Child thread %d created.\n", pthread_self());

	while (true) {
		sem_wait(semph);
		printf("Child Thread %d unblocked.\n", pthread_self());
		sleep(5);
	}
	return NULL;
}

