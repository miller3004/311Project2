#include <iostream>
#include <string>
#include <cstdio>
#include <cerrno>
#include <cunistd>
#include <sys/socket.h>
#include <cstdlib>
#include <sys/sysinfo.h>
//using namespace std;

void handleConnection (int clientFD){
    File * connection = fdopen(clientFD, "rw");
    char * path = NULL;
    size_t capacity = 0;
    ssize_t length;
    char * search = NULL;
    getline(&path, &capacity, connection);
    getline(&search, &capacity, connection);
    std::clog << "PATH: " << path << std::endl;
    std::clog << "SEEKING: " << search << std::endl;
    FILE * f = fopen(path, "r");
    if(f == NULL){
        fprintf(connection, "INVALID FILE\n");
        return;
    }
    char * line = NULL;
    int nLines = 1;
    int sent = 0;
    while((length = getline(&line, &capacity, f)) > 0){
        char * found = strstr(line, search);
        if(found == NULL){
            continue;
        }
        length = fwrite(line, length, 1, connection);
        nLines++;
        sent += length;
    }
}

int main(int argc, char ** argv)
{
    if(argc != 1){
        fprinf(stderr, "Usage: text-server [socket name]\n");
        return EXIT_FAILURE;
    }
    char * socket_name = argv[1];
    int maxClients = get_nprocs_conf()-1;
    //add a check for max clients being 0
    std::clog << "SERVER STARTED\tMAX CLIENTS: " << maxClients << std::endl;
    struct sockaddr_un addr;
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("Server socket fd = %d\n". sfd);

    if(sfd == -1){
        return EXIT_FAILURE;
    }

    if(strlen(socket_name) > sizeof(addr.sun_path) - 1){
        std::cerr << "Server socket path too long: " << socket_name << std::endl;
        return EXIT_FAILURE;
    }

    if(remove(socket_name) == -1 && errno != ENOENT) {
        std::cerr << "remove-%s", socket_name << std::endl;
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
    ssize numRead;
    char buf[BUF_SIZE];
    for(;;) {
        printf("Waiting to accept a connection...\n");
        int cfd = accept(sfd, NULL, NULL);
        printf("Accepted socket fd = %d\n", cfd);
        std::clog << "CLIENT CONNECTED" << std::endl;
        handleConnection(cfd);
        if(close(cfd) == -1){
            std::cerr << "close" << std::endl;
            return EXIT_FAILURE;
        }
    }
}