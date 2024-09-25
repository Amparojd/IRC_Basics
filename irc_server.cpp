#include "irc_server.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h> // Para close
#include <sys/select.h> // Para select
#include <cstdlib> // Para atoi
#include <vector>
#include <string>

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
    listen(serverSocket, 3);
}

void IRCServer::start() {
    std::cout << "Servidor IRC en funcionamiento en el puerto " << port << "..." << std::endl;
    acceptConnections();
}

void IRCServer::acceptConnections() {
    fd_set readfds;
    while (true) {
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);
        int max_sd = serverSocket;

        // Agregar sockets de clientes a la lista de descriptores
        for (size_t i = 0; i < clientSockets.size(); ++i) {
            FD_SET(clientSockets[i], &readfds);
            if (clientSockets[i] > max_sd) {
                max_sd = clientSockets[i];
            }
        }

        // Esperar a que haya actividad en los sockets
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            std::cerr << "Error en select()" << std::endl;
            continue;
        }

        // Si hay actividad en el socket del servidor, aceptar un nuevo cliente
        if (FD_ISSET(serverSocket, &readfds)) {
            struct sockaddr_in clientAddress;
            socklen_t addrlen = sizeof(clientAddress);
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &addrlen);
            if (clientSocket < 0) {
                std::cerr << "Error al aceptar la conexión" << std::endl;
                continue;
            }
            std::cout << "Conexión aceptada." << std::endl;

            // Agregar el nuevo socket a la lista
            clientSockets.push_back(clientSocket);
            clientNicks.push_back(""); // Espacio para el nick del nuevo cliente

            // Enviar mensaje de bienvenida y pedir el nick
            send(clientSocket, "Bienvenido al servidor IRC! Introduce tu nick:\n", 48, 0);
        }

        // Manejar la actividad en los sockets de los clientes
        for (size_t i = 0; i < clientSockets.size(); ++i) {
            int sock = clientSockets[i];
            if (FD_ISSET(sock, &readfds)) {
                handleClient(sock, i); // Pasar el índice para acceder al nick
            }
        }
    }
}

void IRCServer::handleClient(int clientSocket, size_t clientIndex) {
    char buffer[1024] = {0};
    // Si el nick no ha sido recibido todavía
    if (clientNicks[clientIndex].empty()) {
        int valread = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (valread > 0) {
            buffer[valread] = '\0'; // Asegurarse de que la cadena esté terminada
            clientNicks[clientIndex] = std::string(buffer); // Guardar el nick
            std::cout << "Cliente conectado con nick: " << clientNicks[clientIndex] << std::endl;
            send(clientSocket, "Nick registrado.\n", 17, 0);
            return;
        }
    }

    // Manejo de mensajes después de que se ha registrado el nick
    int valread = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (valread <= 0) {
        std::cout << "Cliente " << clientNicks[clientIndex] << " desconectado." << std::endl;
        close(clientSocket);
        clientSockets.erase(clientSockets.begin() + clientIndex);
        clientNicks.erase(clientNicks.begin() + clientIndex);
        return;
    }
    buffer[valread] = '\0'; // Asegurarse de que la cadena esté terminada
    std::cout << "Mensaje recibido de " << clientNicks[clientIndex] << ": " << buffer << std::endl;

    // Enviar respuesta a todos los clientes
    for (size_t i = 0; i < clientSockets.size(); ++i) {
        if (i != clientIndex) { // No enviar al mismo cliente
            send(clientSockets[i], buffer, valread, 0);
        }
    }
}
