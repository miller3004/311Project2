
#include <iostream>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

int main(int argc, char ** argv) 
{
    if(argc != 4)
    {
        fprintf(stderr, "Usage: text-client [socket name file name and search]\n");
        return EXIT_FAILURE;
    }
    struct sockaddr_un addr;
    char* socket_name = argv[1];
    char* file_name = argv[2];
    char* search_string = argv[3];
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sfd == -1)
    {
        std::cerr<<"SOCKET NOT FOUND"<<std::endl;
        return EXIT_FAILURE;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_name, sizeof(addr.sun_path)-2);
    if(connect(sfd,reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == -1)
    {
        std::cerr<<"SERVER CONNECTION NOT ACCEPTED"<<std::endl;
        return EXIT_FAILURE;
    }
    else
    {
	   std::clog << "SERVER CONNECTION ACCEPTED" << std::endl;
    }
    /*FILE* connection = fdopen(sfd, "r+");
    fprintf(connection,"%s %s", file_name, search_string);*/
    std::string info = file_name;
    info += " ";
    info += search_string;
    write(sfd, &info[0], sizeof(info));
    //Writing the file name and search string to the connection
    int nLines = 1;
    size_t recieved = 0;
    char *  line = NULL;
    size_t capacity = 0;
    //ssize_t length = getline(&line, &capacity, connection);
    char buffer[BUFSIZ];
    //for(;;){    
    memset(buffer, 0,BUFSIZ);
    for(;;){
        read(sfd, buffer, sizeof(buffer));
        if(sizeof(buffer== 0)){
            break;
        }
    }  
    std::string input = std::string(buffer);
    recieved = sizeof(input);
    std::cout << input << std::endl;
    std::clog << "BYTES RECIEVED: " << recieved << std::endl;
    return EXIT_SUCCESS;
}
