/* Patrick Czermak 040389514 (czer0005@algonquinlive.com)
 * Lab 1 - Part B
 */

#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

volatile int numChildrenTemp = 0;
volatile int childExitStatus = 0;

void sigint_handler(int sig);
void child_exiting(int temp);
void parent_no_exit(int temp);

int main() {

	pid_t pid;
	int numChildren;

	printf("Enter the number of children: \n");
	scanf(" %d", &numChildren);

	signal(SIGCHLD, child_exiting);

	printf("\nPID = %d : Parent Running...\n", getpid());
	int parentPid = getpid();
	for (int i = 0; i < numChildren; i++) {
		bool forloop = false;

		switch (pid = fork()) {
		case -1:
			perror("fork"); // error somewhere
			exit(1); // parent exits
		case 0:
			printf("PID = %d : Child Running...\n", getpid());
			struct sigaction sa;
			sa.sa_handler = sigint_handler;
			sa.sa_flags = 0;
			sigemptyset(&sa.sa_mask);
			sigaddset(&sa.sa_mask, SIGUSR1);
			if (sigaction(SIGUSR1, &sa, NULL) == -1) {
				perror("process not created");
				exit(1);
			}
			forloop = true;
		} // end switch
		if (forloop) {
			break;
		}
	}
	if (parentPid != getpid()) { // for child
		while (true) {
			if (childExitStatus == getpid()) {
				break;
			}
		}
	} else { // for parent
		// for parent here in order not to kill if children exist
		struct sigaction sa;
		sa.sa_handler = parent_no_exit;
		sa.sa_flags = 0;
		sigemptyset(&sa.sa_mask);
		sigaddset(&sa.sa_mask, SIGUSR1);
		if (sigaction(SIGUSR1, &sa, NULL) == -1) {
			perror("process not created");
			exit(1);
		}
		while (true) {
			if (numChildrenTemp == numChildren) {
				printf("PID = %d : Children finished, parent exiting.\n",
						getpid());
				exit(EXIT_SUCCESS);
				break;
			}
		}
	}
	return 0;
}

// HANDLER for Child processes -> what to display
void sigint_handler(int sig) {
	printf("PID = %d : Child Received USR1.\n", getpid());
	printf("PID = %d : Child Exiting.\n", getpid());
	childExitStatus = getpid();
}

// HANDLER for Child to exit, ensure numChildrenTemp is incremented and used
// to confirm if all child processes have finished
// (to confirm number of children processes exited)
void child_exiting(int temp) {
	numChildrenTemp++;
}

// HANDLER for Parent -> to ensure Parent doesn't exit with child processes
// (child processes must finish first)
void parent_no_exit(int temp) {

}
