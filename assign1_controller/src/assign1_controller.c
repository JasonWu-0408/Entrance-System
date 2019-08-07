#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <process.h>
#include <sys/netmgr.h>
#include <ctype.h>
#include "mystruct.h"


void waiting (void);

int main(int argc, char *argv[]) {

	//Members
	int coid;
	int rcvid;
	int flag = 0;
	int error = 0;
	int comFSM;
	char command[5];

	Display  conMes;
	Display  disMes;
	Person   inputMes;
	State    nextState = START;

	//Get server-PID from input
	pid_t serverpid = atoi(argv[1]);

	//Create Channel for input
	int chid = ChannelCreate(0);
	if(chid == -1){
		perror("failed to create the channel.");
		exit (EXIT_FAILURE);
	}
	printf("The controller is running as PID: %d\n", getpid());

	//Connect to display channel
	coid = ConnectAttach (ND_LOCAL_NODE, serverpid, 1, _NTO_SIDE_CHANNEL, 0);
	if(coid == -1){
		fprintf(stderr, "Couldn't ConnectAttach\n");
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	// Waiting for person
	waiting();
	while(1)
	{
		//Receive person object from input
		rcvid = MsgReceive(chid,&inputMes,sizeof(inputMes),NULL);
		if(rcvid == -1){
			perror("error");
			exit(EXIT_FAILURE);
		}

		strcpy(command, inputMes.command);

		//Process state machine here
		//Controller has a current variable of the current state
		//Person struct has the next input to FSM
		if(strcmp(command,"ls")==0){
			comFSM = LS;
		}else if(strcmp(command,"glu")==0){
			comFSM = GLU;
		}else if(strcmp(command,"lo")==0){
			comFSM = LO;
		}else if(strcmp(command,"ws")==0){
			comFSM = WS;
		}else if(strcmp(command,"lc")==0){
			comFSM = LC;
		}else if(strcmp(command,"gll")==0){
			comFSM = GLL;
		}else if(strcmp(command,"rs")==0){
			comFSM = RS;
		}else if(strcmp(command,"gru")==0){
			comFSM = GRU;
		}else if(strcmp(command,"ro")==0){
			comFSM = RO;
		}else if(strcmp(command,"rc")==0){
			comFSM = RC;
		}else if(strcmp(command,"grl")==0){
			comFSM = GRL;
		}else{
			comFSM = EXIT;
		}


		switch(comFSM)
		{
		case LS:
			if(inputMes.direction == 1){ //Person is entering

				if(nextState != START){
					break;
				}
				conMes.message = L_SCAN;
				conMes.human = inputMes;
				nextState = UNLOCK_LEFT;
			}
			else{
				flag = 1;
				error = 1;
			}
			break;

		case GLU:
			if(nextState == UNLOCK_LEFT)
			{
				conMes.message = L_UNLOCK;
				conMes.human = inputMes;
				nextState = LEFT_OPEN;
			}
			else
			{
				flag = 1;
				error = 1;
			}
			break;

		case LO:
			if(inputMes.direction == 1) //Person is entering
			{
				if(nextState == LEFT_OPEN)
				{
					conMes.message = L_OPEN;
					conMes.human = inputMes;
					nextState = WEIGHT_SCALE;
				}
				else
				{
					flag = 1;
					error = 1;
				}
			}
			else 	//person is leaving
			{
				if(nextState == LEFT_OPEN)
				{
					conMes.message = L_OPEN;
					conMes.human = inputMes;
					nextState = LEFT_CLOSE;
				}
				else
				{
					flag = 1;
					error = 1;
				}
			}

			break;

		case WS:
			if(inputMes.direction == 1)  //Person is entering
			{
				if(nextState == WEIGHT_SCALE){
					conMes.message = W_SCALE;
					conMes.human = inputMes;
					nextState = LEFT_CLOSE;
				}else{
					flag = 1;
					error = 1;

				}
			}
			else //Person is leaving
			{
				if(nextState == WEIGHT_SCALE){
					conMes.message = W_SCALE;
					conMes.human = inputMes;
					nextState = RIGHT_CLOSE;
				}else{
					flag = 1;
					error = 1;
				}
			}
			break;

		case LC:

			if(nextState == LEFT_CLOSE)
			{
				flag = 1;
				nextState = LEFT_LOCK;
			}
			else
			{
				flag = 1;
				error = 1;
			}
			break;


		case GLL:
			if(inputMes.direction == 1) //Person is entering
			{
				if(nextState == LEFT_LOCK)
				{
					conMes.message = L_LOCK;
					conMes.human = inputMes;
					nextState = UNLOCK_RIGHT;
				}
				else{
					flag = 1;
					error = 1;
				}

			}
			else { //Person is leaving
				if(nextState == LEFT_LOCK)
				{
					conMes.message = L_LOCK;
					conMes.human = inputMes;
					nextState = START;
				}else
				{
					flag = 1;
					error = 1;
				}
			}
			break;

		case RS:
			if(inputMes.direction == 0){ //Per
				if(nextState != START){
					break;
				}
				conMes.message = R_SCAN;
				conMes.human = inputMes;
				nextState = UNLOCK_RIGHT;
			}
			else{
				flag = 1;
				error = 1;
			}
			break;

		case GRU:
			if(nextState == UNLOCK_RIGHT){
				conMes.message = R_UNLOCK;
				conMes.human = inputMes;
				nextState = RIGHT_OPEN;
			}else{
				flag = 1;
				error = 1;
			}
			break;

		case RO:
			if(inputMes.direction== 1) //Person is entering
			{
				if(nextState == RIGHT_OPEN)
				{
					conMes.message = R_OPEN;
					conMes.human = inputMes;
					nextState = RIGHT_CLOSE;
				}
				else
				{
					flag = 1;
					error = 1;
				}
			}
			else 	//person is leaving
			{
				if(nextState == RIGHT_OPEN)
				{
					conMes.message = R_OPEN;
					conMes.human = inputMes;
					nextState = WEIGHT_SCALE;
				}
				else
				{
					flag = 1;
					error = 1;
				}
			}
			break;

		case RC:
			if(nextState == RIGHT_CLOSE)
			{
				flag = 1;
				nextState = RIGHT_LOCK;
			}
			else
			{
				flag = 1;
				error = 1;
			}
			break;

		case GRL:
			if(inputMes.direction == 1) //Person is entering
			{
				if(nextState == RIGHT_LOCK)
				{
					conMes.message = R_LOCK;
					conMes.human = inputMes;
					nextState = START;
				}else
				{
					flag = 1;
					error = 1;
				}
			}
			else  //Person is leaving
			{
				if(nextState == RIGHT_LOCK)
				{
					conMes.message = R_LOCK;
					conMes.human = inputMes;
					nextState = UNLOCK_LEFT;
				}else
				{
					flag = 1;
					error = 1;
				}
			}
			break;

		case EXIT:
			conMes.message = SHUTDOWN;
			conMes.human = inputMes;
			break;
		}



		if(flag != 1){
			//send message to display?
			if(MsgSend(coid, &conMes, sizeof(conMes), &disMes, sizeof(disMes)) == -1){
				fprintf (stderr, "Error during MsgSend\n");
				perror (NULL);
				exit (EXIT_FAILURE);
			}
		}

		if(nextState == START)
			waiting();


		//Reply to input;EOK
		MsgReply(rcvid, EOK, &error, sizeof(error));


		flag = 0;
		error = 0;

		if(strcmp(inputMes.command,"exit") == 0)
		{
			printf("Exiting controller\n");
			break;
		}

	}

	//Detach from Channel to display and Destroy Channel to input
	ConnectDetach(coid);
	ChannelDestroy(chid);

	return EXIT_SUCCESS;
}
void waiting (void){
	printf("Waiting for Person...\n");
}




















