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
	srand(time(NULL));
	int server, portNumber;
	int clientScore[2],
	struct sockaddr_in servAdd;     // server socket address
	char message[255];
	
	if(argc != 3){
    printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
	}
	
	if((server = socket(AF_INET, SOCK_STREAM, 0))<0){
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}
	
	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);

	if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}
	if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
		fprintf(stderr, "connect() has failed, exiting\n");
		exit(3);
	}
	
	read(server, message, 255);
	fprintf(stderr, "message received: %s\n", message);
	while(1){ /* reading server’s messages */
		if(n=read(server, message, 255)){
			message[n]=’\0’;
			fprintf(stderr,"%s\n", message);
		}
		if(!strcasecmp(message, "You can now play\n")){
			clientScore[0] = (rand() % 6)+1;
			clientScore[1] +=  clientScore[0];
		}
		printf("Points earned: %d\n", clientScore[0]);
		printf("Total: %d\n", clientScore[1]);
		printf("\n");	
		write(server, &clientScore, sizeof(clientScore));
		
		if(!strcasecmp(message, "Game over: you won the game\n")){
				fprintf("I won the game" );
				close(server);
				exit(3);
		}
		
		if(!strcasecmp(message, "Game over: you lost the game\n")){
				fprintf("I lost the game" );
				close(server);
				exit(5);
		}
		
	}
	
	
}
