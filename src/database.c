extern void error(const char *msg);

void addDataToBuffer(char* buffer, char* data, size_t dataSize, size_t* offset){
	memcpy(buffer + *offset, data, dataSize);
	*offset += dataSize;
}

void handshakeResponse41(void){
	int bufferSize = 1024 * 1024; //1MB
	size_t offset = 0;

	char* buffer = (char*)malloc(bufferSize); 
	memset(buffer, 0, bufferSize);

	//handshake props
	//client_flag
	uint32_t clientCapabilities = 0x00000000;
	uint32_t clientCapabilitiesMask = (1 << 0) | (1 << 2) | (1 << 7) | ( 1<< 9) | (1 << 10);

	clientCapabilities |= clientCapabilitiesMask;

	char clientCapabilitiesArray[sizeof(uint32_t)];
	memcpy(clientCapabilitiesArray, &clientCapabilities, sizeof(uint32_t));

	addDataToBuffer(buffer, &clientCapabilitiesArray,sizeof(uint32_t), &offset);

	//max_packet_size
	uint32_t maxPackageSize = 0x01000000; //quase...16MB
	char maxPackageSizeArray[sizeof(uint32_t)];
	memcpy(maxPackageSizeArray, &maxPackageSize, sizeof(uint32_t));
	addDataToBuffer(buffer, &maxPackageSizeArray,sizeof(uint32_t), &offset);

	
	
}

void databaseConnect(int sockfd, struct sockaddr* address, int addressSize){
	int maxBufferSize = 1024 * 1024; //1MB
	char* buffer = (char*)malloc(maxBufferSize); 

	if(connect(sockfd, address, addressSize) != 0 ){
		error("Error: cannot connect in the database socket"); 
	}

	int bufferSize = recv(sockfd, buffer, maxBufferSize, 0);
	
	if(bufferSize < 0){
		error("database not connect");
	}

/*	for(int i = 0; i < bufferSize; i++){
		if( i > 0) printf(":");
		printf("%04X", (unsigned int)buffer[i]);  
	}
		printf("\n");
*/
	handshakeResponse41();

	free(buffer);
}
