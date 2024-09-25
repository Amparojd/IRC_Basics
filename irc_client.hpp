#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class IRCClient {
public:
    IRCClient(const std::string& address, int port);
    void connectToServer();
    void startChat();

private:
    int clientSocket;
    struct sockaddr_in serverAddress;
    std::string serverAddressStr;
    int port;
    std::string nick;
};

#endif
