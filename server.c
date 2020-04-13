int main(int argc, char *argv[]){  
	int sd, portNumber;
	struct sockaddr_in servAdd;     // server socket address
	
	if(argc != 2){
		printf("Call model: %s <Port Number>\n", argv[0]);
		exit(0);
  }

	sd = socket(AF_INET, SOCK_STREAM, 0);
	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);

	bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
	listen(sd, 5);
	if(listen(sd, 5)){
		printf("Server is listening");
	}
	
	while(1){

	}
	
	return 0;
}