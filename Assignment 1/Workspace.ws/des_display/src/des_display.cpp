#include "../../des_controller/src/des.h"
using namespace std;

int main() {

	int chid = ChannelCreate(0);
	if (chid == -1) {
		perror("Failed to create the channel");
		exit (EXIT_FAILURE);
	}
	printf("Display PID: %d\n", getpid());

	message from_controller;
	int rcvid;

	while (true) {
		rcvid = MsgReceive(chid, &from_controller, sizeof(from_controller),
				NULL);

		if (from_controller.state == ER) {
			printf("%s", toDisplay[invalid]);
		} else {
			printf("%s", toDisplay[from_controller.input]);

			if (from_controller.input == ls || from_controller.input == rs) {
				printf("%d", from_controller.id);
			} else if (from_controller.input == _ws) {
				printf("%d", from_controller.weight);
			}
		}

		MsgReply(rcvid, EOK, NULL, 0);

		if (from_controller.state == EXIT) {
			printf(" Display\n");
			break;
		}
		printf("\n");
	}
	return 0;
}
