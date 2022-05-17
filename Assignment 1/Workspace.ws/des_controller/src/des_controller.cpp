
#include "des.h"
using namespace std;

States get_next_state(States state, Inputs input);

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("Usage: ./des_controller <des_display-PID>");
		exit(EXIT_FAILURE);
	}

	int display_pid = atoi(argv[1]);

	int coid = ConnectAttach(ND_LOCAL_NODE, display_pid, 1, _NTO_SIDE_CHANNEL,
			0);
	if (coid == -1) {
		printf("Could not connect to display");
		exit(EXIT_FAILURE);
	}

	int chid = ChannelCreate(0);
	if (chid == -1) {
		printf("Failed to create the channel");
		exit(EXIT_FAILURE);
	}
	printf("Controller PID: %d\n", getpid());

	message from_input;
	message to_display;
	int rcvid;
	int msgSendSuccess;

	States currentState = SS;
	States tempState = SS;

	while (true) {
		rcvid = MsgReceive(chid, &from_input, sizeof(from_input), NULL);

		tempState = get_next_state(currentState, from_input.input);
		if (tempState != ER) {
			currentState = tempState;
		}
		to_display = from_input;
		to_display.state = tempState;

		msgSendSuccess = MsgSend(coid, &to_display, sizeof(to_display), NULL,
				0);
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

	ChannelDestroy(chid);
	ConnectDetach(coid);

	return 0;
}

States get_next_state(States state, Inputs input) {
	return transitionTable[input][state];
}

