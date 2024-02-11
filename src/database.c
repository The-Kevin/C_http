extern void error(const char *msg);
extern void debugLog(const char *msg);

struct payload {
	int length;
	char* buffer;
};

void addDataToBuffer(char* buffer, char* data, size_t dataSize, size_t* offset){
	memcpy(buffer + *offset, data, dataSize);
	*offset += dataSize;
}

void mysqlEncapsulate(char* buffer, int bufferSize, int sequenceId){
	if(buffer == NULL || bufferSize < 1) error("mysqlEncapsulate error: buffer's empty");

	int header = (bufferSize << 8) | (sequenceId & 0xFF);
	memmove(buffer + sizeof(int), buffer, bufferSize); 
	for(int i = 0; i < sizeof(int); i++){
		if(i == sizeof(int) - 1) {
			buffer[i] = header & 0xFF;
			continue;
		}
		buffer[i] = (header >> (32 - (8 * i))) & 0xFF;
	}
}

//struct payload handshakeResponse41(void){
//	int bufferSize = 63; 
//	size_t offset = 0;
//
//	char* buffer = (char*)malloc(bufferSize); 
//	memset(buffer, 0, bufferSize);
//
//	//handshake props
//	//client_flag
//	uint32_t clientCapabilities = 0x00000000;
//	uint32_t clientCapabilitiesMask = (1 << 0) | (1 << 2) | (1 << 7) | ( 1<< 9) | (1 << 10);
//
//	clientCapabilities |= clientCapabilitiesMask;
//
//	char clientCapabilitiesArray[sizeof(uint32_t)];
//	memcpy(clientCapabilitiesArray, &clientCapabilities, sizeof(uint32_t));
//
//	addDataToBuffer(buffer, clientCapabilitiesArray,sizeof(uint32_t), &offset);
//
//	//max_packet_size
//	uint32_t maxPackageSize = 0x01000000; //quase...16MB
//	char maxPackageSizeArray[sizeof(uint32_t)];
//	memcpy(maxPackageSizeArray, &maxPackageSize, sizeof(uint32_t));
//	addDataToBuffer(buffer, maxPackageSizeArray,sizeof(uint32_t), &offset);
//
//	//character_set
//	char characterSet = 0xff;
//	addDataToBuffer(buffer,&characterSet,sizeof(characterSet), &offset);
//
//	//filler (???? wtf, nao faco ideia do pq disso, tem cara de gambiarra)
//	char filler[23] = {0};
//	addDataToBuffer(buffer,&filler,sizeof(filler), &offset);
//
//	//username
//	char* username = "http";
//	addDataToBuffer(buffer,username,strlen(username) * 8, &offset);
//	
//	return buffer;
//}

void databaseConnect(int* sockfd, struct sockaddr* address, int addressSize){
	int maxBufferSize = 1024 * 1024; //1MB
	char* buffer = (char*)malloc(maxBufferSize); 

	if(connect(*sockfd, address, addressSize) != 0 ){
		error("Error: cannot connect in the database socket"); 
	}


	int bufferSize = recv(*sockfd, buffer, maxBufferSize, 0);
	
	if(bufferSize < 0){
		error("database not connect");
	}

//	for(int i = 0; i < sizeof(example + 5); i++){
//		if( i > 0) printf(":");
//		printf("%04X", (unsigned int)test[i]);  
//	}
//		printf("\n");


//	char* handshake = handshakeResponse41();
//
//	mysqlEncapsulate(handshake, 2);
//	send(sockfd,handshake,63, 0);


	free(buffer);
}
