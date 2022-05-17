#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

int main(void) {
	puts("Hello World from QNX Neutrino RTOS!!!"); /* prints Hello World from QNX Neutrino RTOS!!! */

	puts("\n@author Patrick Czermak (czer0005@algonquinlive.com)");
	puts(
			"I'm taking this course to start my second career, first career was Hotel Management for almost 20 years!");
	puts("");

	printf("\n");
	printf("a) My PID is: %d\n", getpid()); /* PID = Process ID */
	printf("b) My Parent's PID is: %d\n", getppid());
	printf("\n");

	puts("Enter 'q' to quit program: ");
	getchar();

	return EXIT_SUCCESS;
}
