#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define DEBUG 1

#define MAX 80
#define PORT 80
#define LIFE 1
#define MAX_PENDING_CONNECTIONS 5

void error(const char *msg) { perror(msg); exit(1); }
void debugLog(const char *msg) { if(DEBUG) printf("%s", msg); }


int main(int argc, char *argv[]){
	
	int bindConnection;
	int sockfd;
	struct sockaddr_in address;

	sockfd = socket(AF_UNSPEC,SOCK_STREAM,0);

	if(sockfd == -1) error("Socket creating falied");
	debugLog("Socket was created");
	
	bzero(&address, sizeof(address));
	
	
	address.sin_family = AF_UNSPEC; //whatever, IPV4 or IPV6
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);
	

 	bindConnection = bind(sockfd,(struct sockaddr *)&address, sizeof(address));
	
	if(bindConnection != 0){
		error("The bind was not established");
	}
	
	debugLog("The server was bind");

	if(listen(sockfd, MAX_PENDING_CONNECTIONS) != 0){
		error("Error on listen");
	}
	debugLog("The server is listening");
	

	return 0;
}

