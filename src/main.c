#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define MAX 80
#define PORT 80
#define LIFE true

void error(const char *msg) { perror(msg); exit(0); }

int main(int argc, char *argv[]){
	
	int sockfd;

	sockfd = socket(AF_INET,SOCK_STREAM,0);

	if(sockfd == -1) error("Socket creating falied");
		
	
	

	return 0;

}
