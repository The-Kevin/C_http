extern void error(const char *msg);


struct httpHeaderData {
	char* method;
	char* uri;
	char* version;
};

struct httpHeaderData* getHttpHeaderData(char* buffer, int bufferSize){
	char method[bufferSize], uri[bufferSize], version[bufferSize];
	struct httpHeaderData* data = malloc(sizeof(struct httpHeaderData));


	if(data == NULL){
		error("Error: erro ao alocar memoria para header data");
	}
	
	sscanf(buffer, "%s %s %s", method, uri, version);
	
	data->method = method;
	data->uri = uri;
	data->version = version;

	return data;	
}

char* httpRequest(int* sockfd){ //please, remember free() the memory later
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
