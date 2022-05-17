#include "../../des_controller/src/des.h"
using namespace std;

int main() {

	//int chid = ChannelCreate(0);
	name_attach_t *attach;

	if ((attach = name_attach(NULL, "display", 0)) == NULL) {
		perror("Failed to create the channel");
		return EXIT_FAILURE;
	}

	//if (chid == -1) {
	//	perror("Failed to create the channel");
	//	exit (EXIT_FAILURE);
	//}
	printf("Display PID: %d\n", getpid());

	message from_controller;
	int rcvid;

	while (true) {
		//rcvid = MsgReceive(chid, &from_controller, sizeof(from_controller),
		//		NULL);
		rcvid = MsgReceive(attach->chid, &from_controller,
				sizeof(from_controller), NULL);

		if (rcvid == -1) {
			printf("Error, unable to receive message!");
			break;
		}
		if (rcvid == 0) {
			switch (from_controller.hdr.code) {
			case _PULSE_CODE_DISCONNECT:

				printf("client %d is disconnecting. \n",
						from_controller.hdr.scoid);
				ConnectDetach(from_controller.hdr.scoid);
				break;

			case _PULSE_CODE_UNBLOCK:
				printf("Unblock");
				break;

			default:
				printf("Default switch");
				break;
			}
			continue;
		}

		//if (from_controller.state == ER) {
		//	printf("%s", toDisplay[invalid]);
		//} else {
		//	printf("%s", toDisplay[from_controller.input]);

		//	if (from_controller.input == ls || from_controller.input == rs) {
		//		printf("%d", from_controller.id);
		//	} else if (from_controller.input == _ws) {
		//		printf("%d", from_controller.weight);
		//	}
		//}

		//MsgReply(rcvid, EOK, NULL, 0);

		if (from_controller.hdr.type == _IO_CONNECT) {
			printf("IO_CONNECT\n");
			MsgReply(rcvid, EOK, NULL, 0);
			continue;
		}
		if (from_controller.hdr.type == 0x01) {
			if (from_controller.state == ER) {
				printf("%s", toDisplay[invalid]);
			} else {
				printf("%s", toDisplay[from_controller.input]);
				if (from_controller.input == ls
						|| from_controller.input == rs) {
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
			continue;
		}
		if (from_controller.hdr.type > _IO_BASE
				&& from_controller.hdr.type <= _IO_MAX) {
			MsgError(rcvid, ENOSYS);
			continue;
		}

	}
	return 0;
}
