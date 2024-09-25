#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class IRCServer {
public:
    IRCServer(int port);
    void start();

private:
    int serverSocket;
    struct sockaddr_in serverAddress;
    int port;

    void acceptConnections();
    void handleClient(int clientSocket);
};

#endif
