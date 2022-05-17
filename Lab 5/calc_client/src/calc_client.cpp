#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "../../calc_server/src/calc_message.h"

using namespace std;

int main(int argc, char *argv[]) {
	client_send_t msg_send;
	server_response_t msg_receive;
	int coid;
	int server_pid;
	int sent_confirmed;

	if (argc != 5) {
		printf(
				"Usage: ./calc_client <Calc-Server-PID> left_operand operator right_operand");
		exit(EXIT_FAILURE);
	}

	msg_send.left_hand = atoi(argv[2]);
	msg_send.right_hand = atoi(argv[4]);
	server_pid = atoi(argv[1]);
	msg_send.temp_operator = argv[3][0];

	coid = ConnectAttach(ND_LOCAL_NODE, server_pid, 1, _NTO_SIDE_CHANNEL, 0);
	if (coid == -1) {
		printf("Invalid Operator");
		exit(EXIT_FAILURE);
	}

	sent_confirmed = MsgSend(coid, &msg_send, sizeof(msg_send), &msg_receive,
			sizeof(msg_receive));

	if (sent_confirmed == -1) {
		printf("MsgSend had an error.");
		exit(EXIT_FAILURE);
	}

	if (msg_receive.statusCode == SRVR_OK) {
		printf("The server has calculated the result of %d %c %d as %.2f",
				msg_send.left_hand, msg_send.temp_operator, msg_send.right_hand,
				msg_receive.answer);
	} else if (msg_receive.statusCode != SRVR_OK) {
		printf("Error message from server: %s", msg_receive.errorMsg);
	}

	ConnectDetach(coid);
	return EXIT_SUCCESS;
}
