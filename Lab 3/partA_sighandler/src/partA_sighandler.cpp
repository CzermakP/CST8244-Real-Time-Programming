/* Patrick Czermak 040389514 (czer0005@algonquinlive.com)
 * Lab 1 - Part A
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

using namespace std;

volatile sig_atomic_t usr1Happened = false;
void sigint_handler(int sig);

// SIGNAL
int main(void) {

	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);

	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("process not created");
	}
	printf("PID = %d : Running.....\n", getpid());

	while (usr1Happened == false) {

	}
	printf("PID = %d : Exiting.\n", getpid());
	return 0;
}

// HANDLER
void sigint_handler(int sig) {
	printf("PID = %d : Receiving USR1.\n", getpid());
	usr1Happened = 1; // 1 indicates the signal has happened
}
