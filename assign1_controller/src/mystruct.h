/*
 * door_entry.h
 *
 *  Created on: Oct 11, 2018
 *      Author: hurdleg
 *      edit by Tianze
 */

#ifndef DOOR_ENTRY_H_
#define DOOR_ENTRY_H_

#define NUM_STATES 13	// where n = number of states in your finite state machine (FSM)
typedef enum {
	//TODO :: to see the list of //TODOS, Window > Show View > Tasks
	//TODO :: assign an enum value, one for each state
	START,
	LEFT_SCAN,
	UNLOCK_LEFT,
	LEFT_OPEN,
	LEFT_CLOSE,
	LEFT_LOCK,
	RIGHT_SCAN,
	UNLOCK_RIGHT,
	RIGHT_OPEN,
	RIGHT_CLOSE,
	RIGHT_LOCK,
	WEIGHT_SCALE,
	EXIT_EXIT

} State;

#define NUM_INPUTS 12	// where n equals the number of input commands that drive the FSM.
						// From the assignment specification, notice the prompt. Each command, ls, rs, etc.
						// Count those...
typedef enum {
	//TODO :: assign an enum value, one for each input command
	LS,    //0
	GLU,   //1
	LO,	   //2
	LC,    //3
	GLL,   //4
	WS,    //5
	RS,    //6
	RO,    //7
	RC,    //8
	GRU,   //9
	GRL,   //10
	EXIT   //11
} Input;

const char *inMessage[NUM_INPUTS] = {
	//TODO :: each input command. For example, "ls"
	"ls",
	"glu",
	"lo",
	"lc",
	"gll",
	"ws",
	"rs",
	"ro",
	"rc",
	"gru",
	"grl",
	"exit"

};

#define NUM_OUTPUTS 11	// where n equals the number of output messages from the FSM.
typedef enum {
	//TODO :: assign an enum value, one for each output message from the FSM
	WAIT_PERSON,
	L_SCAN,
	L_UNLOCK,
	L_OPEN,
	L_LOCK,
	W_SCALE,
	R_SCAN,
	R_OPEN,
	R_UNLOCK,
	R_LOCK,
	SHUTDOWN
} Output;

const char *outMessage[NUM_OUTPUTS] = {
	//TODO :: each output message. For example, "Person opened left door"
	"Waiting for Person...",
	"Person scanned ID at left door, ID = ",
	"Left door unlocked by Guard",
	"Person opened left door",
	"Left door locked by Guard",
	"Person weighed, Weight = ",
	"Person scanned ID at Right door, ID = ",
	"Person opened right door",
	"Right door unlocked by Guard",
	"Right door locked by Guard",
	"EXIT "
};

// inputs client sends a Person struct to the controller
typedef struct {
	//TODO :: fields for person's ID (int), weight (int), direction (inbound or outbound),
	//			and some way to remember what state the Person is in. Suppose the Person in "Left Scan" state.
	//			You need a way to represent that.
	int id;
	int weight;
	int direction = -1; //1 is in-bound, 0 is out-bound
	char command[20];
} Person;

// controller client sends a Display struct to its server, the display
typedef struct {
	//TODO :: fields for the output message and the Person. For the output message, I used an int, which
	//			is the index into the outMessages array. That way I don't pass strings in my messages.
	//			The Person field is needed, as some output message require information from the Person.
	//			Specifically, those messages that display the Person's ID and weight.
	Person human;
	int message;

} Display;

#endif /* DOOR_ENTRY_H_ */
