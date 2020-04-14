#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> 
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>

int main(int argc, char *argv[])
{	
	srand(time(NULL)); //generate a new batch of pseudo-random numbers
	signal(SIGINT, SIG_IGN);
	int server, portNumber, n;
	int clientScore[2];
	struct sockaddr_in servAdd;     // server socket address
	char message[255];
	
	if(argc != 3){ //checks whether port number and server's IP address is passes as command line arguement
		printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
		exit(0);
	}
	
	if((server = socket(AF_INET, SOCK_STREAM, 0))<0){ //checks whether socket is created or not
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}
	
	servAdd.sin_family = AF_INET; //Domain for Internet, AF -> Address Family
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber); //host to network for short integer : port number

	if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){ // checks whether input is a valid IPv4 dotted-decimal string or not
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}
	if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){ //checks whether the connection with server is established or not
		fprintf(stderr, "connect() has failed, exiting\n");
		exit(3);
	}
	clientScore[0] = 0;
	clientScore[1] = 0;
	while(1){ 
		if(n=read(server, message, 50)){ 	//reads message from server
			message[n]='\0';

			if(!strcasecmp(message, "You can now play")){		//checks server message if its client's turn
				clientScore[0] = (rand() % 6 ) + 1;
				clientScore[1] +=  clientScore[0];
				printf("Points earned: %d\n", clientScore[0]);		//prints score to the screen
				printf("Total points earned: %d\n", clientScore[1]);
				printf("\n");	
				write(server, &clientScore, sizeof(clientScore));	//sending its score to server 
			} else if(!strcasecmp(message, "Game over: you won the game")){		//checks server message if client has won
				fprintf(stderr, "%s\n","I won the game");
				close(server);
				exit(3);
			}else if(!strcasecmp(message, "Game over: you lost the game")){		//checks server message if client has lose
				fprintf(stderr, "%s\n","I lost the game");
				close(server);
				exit(5);
			}
		}
		
	}	
}
