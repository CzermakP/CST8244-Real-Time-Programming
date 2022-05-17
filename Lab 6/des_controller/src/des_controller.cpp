#include "des.h"
using namespace std;

States get_next_state(States state, Inputs input);

int main(int argc, char *argv[]) {

	//if (argc != 2) {
	//	printf("Usage: ./des_controller <des_display-PID>");
	//	exit(EXIT_FAILURE);
	//}

	//int display_pid = atoi(argv[1]);

	int coid = name_open("display", 0);
	if (coid == -1) {
		printf("Could not connect to display");
		exit(EXIT_FAILURE);
	}

	//int chid = ChannelCreate(0);
	//if (chid == -1) {
	//	printf("Failed to create the channel");
	//	exit(EXIT_FAILURE);
	//}
	//printf("Controller PID: %d\n", getpid());
	name_attach_t *attach;
	if ((attach = name_attach(NULL, "controller", 0)) == NULL) {
		perror("Failed to create the channel");
		return EXIT_FAILURE;
	}
	printf("Controller PID: %d\n", getpid());

	message from_input;
	message to_display;
	int rcvid;
	int msgSendSuccess;

	States currentState = SS;
	States tempState = SS;

	while (true) {
		rcvid = MsgReceive(attach->chid, &from_input, sizeof(from_input), NULL);

		if (rcvid == -1) {
			printf("Error, unable to receive message!");
			break;
		}
		if (rcvid == 0) {
			switch (from_input.hdr.code) {
			case _PULSE_CODE_DISCONNECT:
				printf("client %d is disconnecting. \n", from_input.hdr.scoid);
				ConnectDetach(from_input.hdr.scoid);
				break;

			case _PULSE_CODE_UNBLOCK:
				printf("unblocked");
				break;
			default:
				printf("Default switch");
				break;
			}
			continue;
		}

		if (from_input.hdr.type == _IO_CONNECT) {
			printf("IO_CONNECT\n");
			MsgReply(rcvid, EOK, NULL, 0);
			continue;
		}

		if (from_input.hdr.type == 0x01) {
			tempState = get_next_state(currentState, from_input.input);
			if (tempState != ER) {
				currentState = tempState;
			}
			to_display = from_input;
			to_display.state = tempState;

			to_display.hdr.type = 0x01;

			msgSendSuccess = MsgSend(coid, &to_display, sizeof(to_display),
					NULL, 0);
			if (msgSendSuccess == -1) {
				printf("MsgSend had an error.");
				exit(EXIT_FAILURE);
			}

			MsgReply(rcvid, EOK, NULL, 0);

			if (currentState == EXIT) {
				printf("%s Controller\n", toDisplay[ex]);
				break;
			}
		}
	}

	name_detach(attach, 0);
	name_close(coid);

	return 0;
}

States get_next_state(States state, Inputs input) {
	return transitionTable[input][state];
}

