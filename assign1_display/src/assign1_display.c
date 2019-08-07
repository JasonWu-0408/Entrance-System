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

int main(void) {

	//Declare members
	Display show;  //has 'human and message' inside
	int rcvid = 0;

	//Create Channel
	int chid = ChannelCreate(0);
	if(chid == -1){
		perror("failed to create the channel.");
		exit (EXIT_FAILURE);
	}
	printf("The display is running as PID: %d\n", getpid());

	//Infinity loop for receive message from controller
	while(1)
	{
		//Receive a display object from controller
		rcvid = MsgReceive(chid,&show,sizeof(show),NULL);
		if(rcvid == -1){
			perror("error");
			exit(EXIT_FAILURE);
		}

		//Reply to Controller;EOK
		MsgReply(rcvid, EOK, NULL, 0);

		//Display the message inside
		if(show.message != SHUTDOWN)
		{
			printf("%s",outMessage[show.message]);

			if(show.message == L_SCAN || show.message == R_SCAN)
			{
				printf("%d", show.human.id);
			}
			else if(strcmp(outMessage[show.message],"Person weighed, Weight = ") == 0)
			{
				printf("%d", show.human.weight);
			}
			printf("\n");
		}
		else{
			printf("Exiting Display.\n");
			break;
		}
	}
	//Destroy the channel
	ChannelDestroy(chid);

	return EXIT_SUCCESS;
}


