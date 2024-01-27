extern void error(const char *msg);


char* httpResquest(int* sockfd){ //please, remember free() the memory later
	char* buffer = malloc(2064); // 256bytes
	if(buffer == NULL){ error("Error: buffer malloc error in httpResponse function");};

	ssize_t bytesRead = recv(*sockfd, buffer,sizeof(buffer), 0);

	if(bytesRead <= 0) {error("Error: cannot reading socket");}
	
	buffer[bytesRead] = '\0';
	return buffer;
}

void httpResponse(int* sockfd, const char* msg){

	send(*sockfd, msg, strlen(msg), 0);
	
}
