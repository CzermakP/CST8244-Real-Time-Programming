#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include "calc_message.h"

using namespace std;

int main(int argc, char *argv[]) {
	client_send_t client_message;
	server_response_t response;
	int rcvid;
	int chid;

	chid = ChannelCreate(0);
	if (chid == -1) {
		perror("Not able to create channel!");
		exit(EXIT_FAILURE);
	}
	printf("CalcServer PID: %d\n", getpid());

	while (true) {
		rcvid = MsgReceive(chid, &client_message, sizeof(client_message), NULL);
		response.statusCode = SRVR_OK;
		response.errorMsg[0] = '\0';

		switch (client_message.temp_operator) {
		case '+': //overflow
			if (client_message.left_hand > 0 && client_message.right_hand > (INT_MAX - client_message.left_hand)) {
				response.statusCode = SRVR_OVERFLOW;
				string message = "OVERFLOW: ";
				message += to_string(client_message.left_hand) + " ";
				message += client_message.temp_operator;
				message += " ";
				message += to_string(client_message.right_hand);
				strcat(response.errorMsg, message.c_str());
			}
			else {
				response.answer = client_message.left_hand + client_message.right_hand;
			}
			break;

		case '-':
			response.answer = client_message.left_hand - client_message.right_hand;
			break;

		case 'x': //overflow
			if (client_message.left_hand > 0 && client_message.right_hand > (INT_MAX - client_message.left_hand)) {
				response.statusCode = SRVR_OVERFLOW;
				string message = "OVERFLOW: ";
				message += to_string(client_message.left_hand) + " ";
				message += client_message.temp_operator;
				message += " ";
				message += to_string(client_message.right_hand);
				strcat(response.errorMsg, message.c_str());
			}
			else {
				response.answer = client_message.left_hand * client_message.right_hand;
			}
			break;

		case '/':
			if (client_message.right_hand == 0) {
				response.statusCode = SRVR_UNDEFINED;
				string message = "UNDEFINED: ";
				message += to_string(client_message.left_hand) + " ";
				message += client_message.temp_operator;
				message += " ";
				message += to_string(client_message.right_hand);
				strcat(response.errorMsg, message.c_str());
			}
			else {
				response.answer = (double)client_message.left_hand / (double)client_message.right_hand;
			}
			break;

		default:
			response.statusCode = SRVR_INVALID_OPERATOR;
			string message = "INVALID OPERATOR: ";
			message += client_message.temp_operator;
			strcat(response.errorMsg, message.c_str());
			break;
		}
		MsgReply(rcvid, EOK, &response, sizeof(server_response_t));
	}
	return 0;
}

