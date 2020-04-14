/*  Online DiceGame

	Team names:
	Khushboo Mehta - 110005899
	Aditi Desai - 110005940
	
	Guided By:
	Dr. Boubakeur Boufama
	Dr. Osama Hamzeh */
	
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

void servicePlayers(int, int);

int main(int argc, char *argv[]){  
	int sd, portNumber, player1, player2;
	struct sockaddr_in servAdd;     // server socket address
	
	if(argc != 2){ 		//checks whether port number is passed as command line arguement
		printf("Call model: %s <Port Number>\n", argv[0]);
		exit(0);
  }

	//socket() creates an endpoint for communication
	if((sd = socket(AF_INET, SOCK_STREAM, 0))<0){ 	//checks whether socket is created or not
		fprintf(stderr, "Could not create socket\n");
		exit(1);
  }
	servAdd.sin_family = AF_INET; 	//Domain for Internet, AF -> Address Family
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY); 	//host to network for long integer : INADDR_ANY(binds the socket to all available interfaces)
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber); 	//host to network for short integer : port number 

	//casts socket file descriptor to the protocal-specific address structure
	bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
	
	//listening socket listens the connections requested by client
	listen(sd, 6);
	
	while(1){

		//accepts pending connection requests from queue(FIFO) and create connected socket of each request
		player1=accept(sd,(struct sockaddr*)NULL,NULL);
		player2=accept(sd,(struct sockaddr*)NULL,NULL);
		printf("Two clients joined.\n");

		if(!fork()) //For every 2 players joined, child process is created for a new game between them
		{
			//child process calls servicePlayer method for client 1 and client 2 to play dice game
			servicePlayers(player1, player2);
		}
	}
	
	return 0;
}

void servicePlayers(int player1, int player2){

	int player1Score[2], player2Score[2];
  	player1Score[0]=0;
  	player1Score[1]=0;
  	player2Score[0]=0;
  	player2Score[1]=0;
	
	while(1){
    
		printf("Referee: TOTO plays:\n");
		write(player1, "You can now play", 50);		//sends message to client 1 to play its turn
		if(read(player1, &player1Score, sizeof(player1Score)) < 0){		//reads score from client 1
			fprintf(stderr, "read() error\n");
			exit(3);
		}
		
		printf("Points earned: %d\n", player1Score[0]);		//prints points earned by client 1
		printf("Total points earned: %d\n\n", player1Score[1]);		//prints total points earned by client 1 so far
		
		if(player1Score[1] >= 100){		//checks if total score of client 1 reaches 100
			printf("TOTO won\n");
			write(player1, "Game over: You won the game",50);	//sends winning message to client 1
			close(player1);		//closes client 1 socket
			write(player2, "Game over: you lost the game",50);	//sends losing message to client 2	
			close(player2);		//closes client 2 socket
			exit(0);
		}
		sleep(2);

		printf("Referee: TITI plays:\n");
		write(player2, "You can now play", 50);		//sends message to client 2 to play its turn
		if(read(player2, &player2Score, sizeof(player2Score)) < 0){		//reads score from client 2
			fprintf(stderr, "read() error\n");
			exit(3);
		}
		
		printf("Points earned: %d\n", player2Score[0]);		//prints points earned by client 2
		printf("Total points earned: %d\n\n", player2Score[1]);		//prints total points earned by client 2 so far
		
		if(player2Score[1] >= 100){		//checks if total score of client 2 reaches 100
			printf("TITI won\n");
			write(player2, "Game over: you won the game", 50);	//sends winning message to client 2
			close(player2);		//closes client 2 socket
			write(player1, "Game over: you lost the game", 50);	//sends losing message to client 2
			close(player1);		//closes client 1 socket
			exit(0);
		}
		sleep(2);		
  }
    exit(0); 
}