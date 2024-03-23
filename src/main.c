#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


/*https://dev.mysql.com/doc/dev/mysql-server/latest/page_protocol_connection_phase_packets_protocol_handshake_response.html*/

struct HandshakeResponse41 {
    uint32_t client_flag;
    uint32_t max_packet_size;
    uint8_t  character_set;
    char     filler[23];
    char     username[5]; // always root
    uint8_t  auth_response_length;
    char     auth_response[40];
    char     database[5];   
};

void connectDatabase(int sockFd, struct sockaddr_in* addr, socklen_t addrLen){

    addr->sin_addr.s_addr = inet_addr("127.0.0.1");
    addr->sin_port = htons(3306); //mysql port
    addr->sin_family = AF_INET;

    if(connect(sockFd, (const struct sockaddr*)addr, addrLen) == -1){
        printf("Error to connect in database: %s",strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Mysql handshake
    struct HandshakeResponse41 handshakePacket;
    char filler[23] = {0};
    char sha1Auth[40] = "9d4e1e23bd5b727046a9e3b4b7db57bd8d6ee684";
    char databaseName[5] = "http";
    int s;
    uint32_t capacibilities = 
          0x00000001 //CLIENT_CONNECT_WITH_DB
        | 0x00080000; //CLIENT_PLUGIN_AUTH
    

    handshakePacket.client_flag = htonl(capacibilities);
    handshakePacket.max_packet_size = htonl(0xFFFFFF);
    handshakePacket.character_set = htons(0x21); // utf8mb3_general_ci
    memcpy(handshakePacket.filler, filler, sizeof(filler));
    memcpy(handshakePacket.username, "root",5);
    handshakePacket.auth_response_length = htons(40);
    memcpy(handshakePacket.auth_response, sha1Auth, 40);
    memcpy(handshakePacket.database, databaseName, 5);


 //    s = send(sockFd,&handshakePacket, sizeof(struct HandshakeResponse41),0);
 //    if(s == -1){
 //        printf("Error to send handshake to database: %s",strerror(errno));
 //        exit(EXIT_FAILURE);
 //    }

}

int main(void){

    int mainFd, databaseFd;
    struct sockaddr_in mainAddr, databaseAddr; 

    mainFd = socket(AF_INET, SOCK_STREAM, 0);
    databaseFd = socket(AF_INET, SOCK_STREAM, 0);

    if(mainFd == -1 || databaseFd == -1){
        printf("Error in set socket in filedescriptor: %s",strerror(errno));
        exit(EXIT_FAILURE);
    }

    connectDatabase(databaseFd, &databaseAddr, sizeof(databaseAddr));

    mainAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    mainAddr.sin_family = AF_INET;
    mainAddr.sin_port = htons(80); // http

    if(bind(mainFd, (const struct sockaddr*)&mainAddr,sizeof(mainAddr)) == -1){
        printf("error to bind the fd in the port: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }


    if(listen(mainFd, 3) == -1){
        printf("Error listening on socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for(;;){
        struct sockaddr_in clientAddr; 
        socklen_t clientAddrLen = sizeof(clientAddr);

        int clientFd = accept(mainFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if(clientFd == -1){
            printf("error to accept: %s",strerror(errno));
            exit(EXIT_FAILURE);
        }

        close(clientFd);
    }

    return 0;
}
