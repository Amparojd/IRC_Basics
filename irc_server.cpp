#include "irc_server.hpp"
#include <cstdlib>

IRCServer::IRCServer(int port) : port(port) {
    // Crear el socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error al crear el socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Escuchar en todas las interfaces
    serverAddress.sin_port = htons(port);

    // Vincular el socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error al vincular el socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones
    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Error al escuchar conexiones" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void IRCServer::start() {
    std::cout << "Servidor IRC en funcionamiento en el puerto " << port << "..." << std::endl;
    acceptConnections();
}

void IRCServer::acceptConnections() {
    while (true) {
        int clientSocket;
        struct sockaddr_in clientAddress;
        socklen_t addrlen = sizeof(clientAddress);

        // Aceptar una nueva conexión
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &addrlen);
        if (clientSocket < 0) {
            std::cerr << "Error al aceptar la conexión" << std::endl;
            continue;
        }
        std::cout << "Conexión aceptada." << std::endl;

        handleClient(clientSocket);
        close(clientSocket); // Cerrar el socket del cliente después de manejarlo
    }
}

void IRCServer::handleClient(int clientSocket) {
    char buffer[1024] = {0};
    const char* welcomeMessage = "Bienvenido al servidor IRC!\n";
    send(clientSocket, welcomeMessage, strlen(welcomeMessage), 0);

    while (true) {
        memset(buffer, 0, sizeof(buffer)); // Limpiar el buffer antes de recibir
        int valread = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (valread <= 0) {
            std::cout << "Cliente desconectado." << std::endl;
            break; // Salir del bucle si el cliente se desconecta
        }
        std::cout << "Mensaje recibido: " << buffer << std::endl;
        const char* responseMessage = "Mensaje recibido\n";
        send(clientSocket, responseMessage, strlen(responseMessage), 0);
    }
}
