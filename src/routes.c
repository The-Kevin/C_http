//extern void error(const char *msg);


void routes(char* method, char* uri){
	
	char* GET = "GET";
	char* POST = "POST";
	char* PUT = "PUT";
	char* DELETE = "DELETE";

	if(strcmp(method, GET) == 0 ){
		printf("GET");
	}	

	if(strcmp(method, POST) == 0 ){
		printf("POST");
	}
		
	if(strcmp(method, PUT) == 0 ){
		printf("PUT");
	}	

	if(strcmp(method, DELETE) == 0 ){
		printf("DELETE");
	}	
}

