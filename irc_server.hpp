#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <string>

class IRCServer {
private:
    int serverSocket;
    struct sockaddr_in serverAddress;
    int port;

    void acceptConnections();
    void handleClient(int clientSocket, size_t clientIndex); // Modificado para incluir el índice
    std::vector<int> clientSockets; // Para almacenar sockets de clientes
    std::vector<std::string> clientNicks; // Para almacenar nicks de clientes
public:
    IRCServer(int port);
    void start();
};

#endif
