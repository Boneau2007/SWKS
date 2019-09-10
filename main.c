#include <stdlib.h>
#include <string.h>
#include "Server.h"
#define TCP_ECHO_SERVICE_SERVICE 9001
#define NAMED_PIPE_SERVICE 9002
#define UDP_DATE_SERVICE 9003

typedef enum {start, stop, pause, resume} MODE;

struct service{
	char* name;
	MODE mode;
}

const char* TCP_ECHO_SERVICE = "echoservice";
const char* NAMED_PIPE = "namedpipe";
const char* UDP_DATE = "game";

int chooseService(char*);
int chooseAction(char*);
char* currentState();
int performStateSwitch();

int main(int argc, char** argv) {
	int initValue = -1;
	if (argc != 3) {
		print("usage: main <service> <mode>\n");
		print("service: [echoservice], [namedpipe], [game]\n");
		print("mode: [start], [stop], [pause], [resume]\n");
		print("e.g : main echoservice start\n");
		exit(EXIT_FAILURE);
	}

	int service = chooseService(argv[1]);
	int action = chooseAction(argv[2]);
	char* state = currentState();

	if (service == EXIT_FAILURE || action == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	if (strcmp(action, state)) {
		printf("Service: [%s] befindet sicher bereits in Modus: [%s]", argv[1], argv[2]);
		return EXIT_FAILURE;
	}

	if (!performStateSwitch(service, action)) {
		printf("Service: [%s] kann Zustand nicht von [%s] in [%s] wechseln.", argv[1], state, argv[2]);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int chooseService(char* serviceName) {
	if (strcmp(serviceName, TCP_ECHO_SERVICE)) {
		return TCP_ECHO_SERVICE_SERVICE;
	}
	else if (strcmp(serviceName, NAMED_PIPE)) {
		return NAMED_PIPE_SERVICE;
	}
	else if (strcmp(serviceName, UDP_DATE)) {
		return UDP_DATE_SERVICE;
	}
	else {
		return EXIT_FAILURE;
	}
}

/*
 * Evaluates the second input-parameter if it is start,stop,pause or resume
 */
int chooseAction(char* action) {
	if (strcmp(action, "start")) {
	}
	else if (strcmp(action, "stop")) {
	}
	else if (strcmp(action, "pause")) {
	}
	else if (strcmp(action, "resume")) {
	}
	else {
		return EXIT_FAILURE;
	}
}

/*
 * Checks the current state of the given service in parameter one.
 */
char* currentState() {}

/*
 * Switches the operation mode of the services
 */
int performStateSwitch(int service, int action) {

}
