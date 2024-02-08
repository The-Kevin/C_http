#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "http.c"
#include "routes.c"
#include "database.c"

	
#define DEBUG 1

#define MAX 80
#define PORT 80
#define DATABASE_PORT 3306
#define LIFE 1
#define MAX_PENDING_CONNECTIONS 5

// 16MB
#define MYSQL_PACKET_SIZE 16777216 

void error(const char *msg) { perror(msg); exit(1); }
void debugLog(const char *msg) { if(DEBUG) printf("%s\n", msg); }


int main(int argc, char *argv[]){
	
	socklen_t addr_size;
	int sockfd,newSockfd, bindConnection, sockdatabasefd;
	struct sockaddr_in address, their_addr, databaseAddress;

	const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello corno!";

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	sockdatabasefd	= socket(AF_INET, SOCK_STREAM, 0);


	if(sockfd == -1) error("Socket creating falied");
	if(sockdatabasefd == -1) error("Socket database creating falied");

	debugLog("Socket was created");
	
	bzero(&address, sizeof(address));
	bzero(&databaseAddress, sizeof(databaseAddress));
	
	address.sin_family = AF_UNSPEC; //whatever, IPV4 or IPV6
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);	
	

	//database connection
	inet_pton(AF_INET, "0.0.0.0", &(databaseAddress.sin_addr));
	databaseAddress.sin_family = AF_UNSPEC;
	databaseAddress.sin_port = htons(DATABASE_PORT);


	databaseConnect(sockdatabasefd, (struct sockaddr *)&databaseAddress, sizeof(databaseAddress));
	debugLog("The database is connected in the socket");

 	bindConnection = bind(sockfd,(struct sockaddr *)&address, sizeof(address));
	
	
	if(bindConnection != 0){
		error("The bind was not established");
	}
	
	if(listen(sockfd, MAX_PENDING_CONNECTIONS) != 0){
		error("Error on listen");
	}
	debugLog("The server is listening");

	
	addr_size = sizeof(their_addr);
	newSockfd = accept(sockfd, (struct sockaddr *)&their_addr,&addr_size);
	if( newSockfd < 0){
		error("Accept request field failed");
	}
	debugLog("The server is accepting the requests in the queue");
	
	for(;;){
		char* buffer;
		struct httpHeaderData* header;
		buffer = httpRequest(&newSockfd);
		header = getHttpHeaderData(buffer, sizeof buffer);
		
		routes(header->method, header->uri);
	
		//close(newSockfd);
	}


	close(sockfd);
	return 0;
}

