
#include <iostream>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
//#include <sys/un.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
    if(argc != 4){
        fprintf(stderr, "Usage: text-client [socket name file name and search]\n");
        return EXIT_FAILURE;
    }
    struct sockaddr_un addr;
    char* socket_name = argv[1];
    char* file_name = argv[2];
    char* search_string = argv[3];
    int sfd = socket(AF_UNIX, SOCK_STREAM);
    printf("Client socket fd = %d\n", sfd);
    if(sfd == -1){
        std::cerr<<"socket"<<std::endl;
        return EXIT_FAILURE;
    }
    memset(&addr, 0, sizeof(struct addr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_name, sizeof(addr.sun_path)-1);
    if(connect(sfd, (struct sockaddr*) &addr, sizeof(struct sockaddr_un)) == -1){
        std::cerr<<"connect"<<std::endl;
        return EXIT_FAILURE;
    }
    FILE* connection = fdopen(sfd, "rw");
    fprintf(connection, "%s\n%s\n", file_name, search_string);
    int nLines = 1;
    int recieved = 0;
    char * line = NULL;
    size_t capacity = 0;
    ssize_t length;
    while((length = getLine(&line, &capacity, connection))>0){
        printf("&d\t%s\n",nLines, line);
        nLines++;
        recieved = recieved + length;
    }
    std::clog << "BYTES RECIEVED: " << recieved << std::endl;
    return EXIT_SUCCESS;
}