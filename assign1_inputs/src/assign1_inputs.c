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
#include "../../assign1_controller/src/mystruct.h"

int check(const char*com);

int main(int argc,char *argv[]) {

	Person inputMes;
	int error = 0;
	int coid;
	int userId;
	int userWeight;
	char com[20];

	//Get serverpid from input
	pid_t serverpid = atoi(argv[1]);

	//Set up connection
	coid = ConnectAttach (ND_LOCAL_NODE, serverpid, 1, _NTO_SIDE_CHANNEL, 0);
	if(coid == -1){
		fprintf(stderr, "Couldn't ConnectAttach\n");
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	//Infinity loop for send message to controller
	while(1){

		//Check input command
		do{
			//prompt for the input command
			printf("Enter the event type (ls= left scan, rs= right scan, ws= weight scale, "
					"lo =left open, ro=right open, lc = left closed, rc = right closed , "
					"gru = guard right unlock, grl = guard right lock, gll=guard left lock, "
					"glu = guard left unlock)\n");
			scanf("%s", com);
			strcpy(inputMes.command,com);

		}while(check(com)!=0);

		//Prompt for user input direction and ID
		if(strcmp(com,"ls") == 0 || strcmp(com,"rs") == 0)
		{
			printf("Please input user ID: \n");
			scanf("%d",&userId);
			inputMes.id = userId;
			//"1" is in-bound, "0" is out-bound
			if(strcmp(com,"ls") == 0){
				inputMes.direction = 1;
			}
			else
				inputMes.direction = 0;
		}

		//Prompt for user input weight
		if(strcmp(com,"ws") == 0)
		{
			printf("Enter person's weight: \n");
			scanf("%d",&userWeight);
			inputMes.weight = userWeight;
		}

		//Message send
		if(MsgSend(coid, &inputMes, sizeof(inputMes), &error, sizeof(error)) == -1){
			fprintf (stderr, "Error during MsgSend\n");
			perror (NULL);
			exit (EXIT_FAILURE);
		}

		if(error == 1){
			printf("You entered a wrong event. Please type again\n");
		}

		//Exit the input
		if (strcmp(com,"exit") == 0)
			break;

	}

	//Disconnect Channel
	ConnectDetach(coid);

	return EXIT_SUCCESS;
}

int check(const char *com){

	if(strcmp(com,"ls")==0){
		return 0;
	}else if(strcmp(com,"rs")==0){
		return 0;
	}else if(strcmp(com,"ws")==0){
		return 0;
	}else if(strcmp(com,"lo")==0){
		return 0;
	}else if(strcmp(com,"lc")==0){
		return 0;
	}else if(strcmp(com,"ro")==0){
		return 0;
	}else if(strcmp(com,"rc")==0){
		return 0;
	}else if(strcmp(com,"gru")==0){
		return 0;
	}else if(strcmp(com,"grl")==0){
		return 0;
	}else if(strcmp(com,"gll")==0){
		return 0;
	}else if(strcmp(com,"glu")==0){
		return 0;
	}else if(strcmp(com,"exit")==0){
		return 0;
	}else
		return 1;
}


