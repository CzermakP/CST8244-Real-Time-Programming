#include "../../des_controller/src/des.h"

using namespace std;

enum Inputs get_input(string input);

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("Usage: ./des_inputs <des_controller-PID>");
		exit (EXIT_FAILURE);
	}

	int controller_pid = atoi(argv[1]);

	int coid = ConnectAttach(ND_LOCAL_NODE, controller_pid, 2,
			_NTO_SIDE_CHANNEL, 0);
	if (coid == -1) {
		printf("Could not connect to controller");
		exit (EXIT_FAILURE);
	}

	string userInput = "";
	enum Inputs input;
	int msgSendSuccess;

	while (true) {
		message to_controller;

		printf("Enter the event type: ");
		cin >> userInput;

		input = get_input(userInput);
		to_controller.input = input;

		if (input == ls || input == rs) {
			printf("Enter ID: ");
			cin >> to_controller.id;
		} else if (input == _ws) {
			printf("Enter Weight: ");
			cin >> to_controller.weight;
		}

		// Check to make sure id or weight are integers
		if (cin.fail()) {
			to_controller.input = invalid;
			cin.clear();
		}

		msgSendSuccess = MsgSend(coid, &to_controller, sizeof(to_controller),
				NULL, 0);
		if (msgSendSuccess == -1) {
			printf("MsgSend had an error.");
			exit (EXIT_FAILURE);
		}

		if (input == ex) {
			printf("%s Inputs\n", toDisplay[ex]);
			break;
		}
	}

	ConnectDetach(coid);
	return 0;
}

enum Inputs get_input(string input) {
	enum Inputs ret_input;

	// To lower case
	transform(input.begin(), input.end(), input.begin(), ::tolower);

	if (input == "exit") {
		ret_input = ex;
	} else if (input == "ls") {
		ret_input = ls;
	} else if (input == "rs") {
		ret_input = rs;
	} else if (input == "ws") {
		ret_input = _ws;
	} else if (input == "lo") {
		ret_input = lo;
	} else if (input == "ro") {
		ret_input = ro;
	} else if (input == "lc") {
		ret_input = lc;
	} else if (input == "rc") {
		ret_input = rc;
	} else if (input == "gru") {
		ret_input = gru;
	} else if (input == "grl") {
		ret_input = grl;
	} else if (input == "gll") {
		ret_input = gll;
	} else if (input == "glu") {
		ret_input = glu;
	} else {
		ret_input = invalid;
	}

	return ret_input;
}
