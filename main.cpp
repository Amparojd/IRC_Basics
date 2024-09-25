#include <iostream>
#include "irc_server.hpp"
#include "irc_client.hpp"
#include <cstdlib>

int main(int argc, char** argv) {
    if (argc == 2) {
        // Ejecución del servidor
        int port = std::atoi(argv[1]);
        IRCServer server(port);
        server.start();
    } else if (argc == 3) {
        // Ejecución del cliente
        std::string address = argv[1];
        int port = std::atoi(argv[2]);
        IRCClient client(address, port);
        client.connectToServer();
        client.startChat();
    } else {
        std::cerr << "Uso: ./irc_app <port> para el servidor o ./irc_app <address> <port> para el cliente" << std::endl;
        return 1;
    }
    return 0;
}
