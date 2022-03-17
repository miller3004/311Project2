#include <iostream>
#include <string>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <sys/un.h>

void handleConnection (int clientFD){
    /*FILE * connection = fdopen(clientFD, "r+");
    if(connection == NULL){
	    std::clog << "File can not be found" << std::endl;
    }
    else {
	    std::clog << "File was found" << std::endl;
    }
    char * path = NULL;
    size_t capacity = 0;
    ssize_t length = 1;
    char * search = NULL;

    getdelim(&path, &capacity,' ', connection);
    std::string pathTemp = std::string(path);
    pathTemp.pop_back();
    delete [] path;
    path = &pathTemp[0];

    getline(&search, &capacity, connection);
    std::string searchTemp = std::string(search);
    delete [] search;
    search = &searchTemp[0];

    std::clog << "PATH: " <<  path << std::endl;
    std::clog << "SEEKING: " << search << std::endl;
    FILE * f = fopen(path, "r+");
    if(f == NULL){
        fprintf(connection, "INVALID FILE\n");
	   std::clog << path << std::endl;
	   std::clog << "File not found" << std::endl;
        return;
    }
    char * line = NULL;
    int nLines = 1;
    int sent = 0;
    //length = getline(&line, &capacity, f);
    int linesTemp = 0;
    while(length  >= 0)
    {
        length = getline(&line, &capacity, f);
	    char * found = strstr(line, search);
        if(found == NULL)
        {
            linesTemp++;
            continue;
        }
        //length = fwrite(line, length, 1, connection);
	    //fprintf(connection, "%s", line);
        std::clog << line;
        nLines++;
        sent += length;
    }
    std::clog << "Out of while loop" <<std::endl;*/
    char buffer[BUFSIZ];
    //for(;;){    
        memset(buffer, 0,BUFSIZ);
        read(clientFD, buffer, sizeof(buffer));
        std::string input = std::string(buffer);
        for(int i = BUFSIZ;i>=0; i--){
            if(input[i] == '\n'){
                input.erase(i,1);
            }
        }
        std::string pathTemp = "";
        int cutoff = 0;
        //std::clog << input << std::endl;
        for(size_t i = 0; i < sizeof(input); i++){
            if(input[i] == ' '){
                cutoff = i+1;
                i = sizeof(input);
            }
            else{
                pathTemp = pathTemp + input[i];
            }
        }
        char * path;
        path = &pathTemp[0];

        std::string searchTemp = "";
        for(size_t i = cutoff; i < sizeof(input); i++){
            searchTemp = searchTemp + input[i];
        }
        char * search;
        search = &searchTemp[0];

        std::clog << "PATH: " <<  path << std::endl;
        std::clog << "SEEKING: " << search << std::endl;
        FILE * f = fopen(path, "r+");
        if(f == NULL){
            
            std::clog << path << std::endl;
            std::clog << "File not found" << std::endl;
            return;
        }
        char * line = NULL;
        int nLines = 1;
        int sent = 0;
        ssize_t length = 0;
        int linesTemp = 0;
        size_t capacity = 0;
        std::string clientInfo = "";
        while(length  >= 0)
        {
            length = getline(&line, &capacity, f);
            char * found = strstr(line, search);
            if(found == NULL)
            {
                linesTemp++;
                continue;
            }
            clientInfo += line;
            //write(clientFD, line, length);
            std::clog << line;
            nLines++;
            sent += length;
        }
        write(clientFD, &clientInfo[0], sent);
    //}
}

int main(int argc, char ** argv)
{
    if(argc != 2){
        fprintf(stderr, "Usage: text-server [socket name]\n");
        return EXIT_FAILURE;
    }
    char * socket_name = argv[1];
    int maxClients = get_nprocs_conf()-1;
    //add a check for max clients being 0
    std::clog << "SERVER STARTED\tMAX CLIENTS: " << maxClients << std::endl;
    struct sockaddr_un addr;
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("Server socket fd = %d \n", sfd);

    if(sfd == -1)
    {
	std::clog << "sfd is -1" << std::endl;
        return EXIT_FAILURE;
    }

    if(strlen(socket_name) > sizeof(addr.sun_path) - 1){
        std::cerr << "Server socket path too long: " << socket_name << std::endl;
        return EXIT_FAILURE;
    }

    if(remove(socket_name) == -1 && errno != ENOENT) {
        std::cerr << ("remove-%s", socket_name) << std::endl;
        return EXIT_FAILURE;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_name, sizeof(addr.sun_path) - 1);
    if(bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1){
        std::cerr << "bind" << std::endl;
        return EXIT_FAILURE;
    }

    if(listen(sfd, maxClients) == -1){
        std::cerr << "listen" << std::endl;
        return EXIT_FAILURE;
    }
    ssize_t numRead;
    char buf[BUFSIZ];
    //for(;;) {
        printf("Waiting to accept a connection...\n");
        int cfd = accept(sfd, NULL, NULL);
        printf("Accepted socket fd = %d\n", cfd);
        std::clog << "CLIENT CONNECTED" << std::endl;
        handleConnection(cfd);
        if(close(cfd) == -1){
            std::cerr << "close" << std::endl;
            return EXIT_FAILURE;
        }
    //}
}
