#include "irc_client.hpp"
#include <cstdlib> // Para EXIT_FAILURE y exit
#include <cstring> // Para memset
#include <iostream>
#include <unistd.h> // Para close

IRCClient::IRCClient(const std::string& address, int port) : serverAddressStr(address), port(port) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error al crear el socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(serverAddressStr.c_str());
}

void IRCClient::connectToServer() {
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Conexión fallida" << std::endl;
        exit(EXIT_FAILURE);
    }
    // Pedir el nick al usuario
    std::cout << "Introduce tu nick: ";
    std::getline(std::cin, nick); // Leer el nick

    // Enviar el nick al servidor
    send(clientSocket, nick.c_str(), nick.length(), 0);
    std::cout << "Conectado al servidor\n";
}

void IRCClient::startChat() {
    char buffer[1024] = {0};

    while (true) {
        std::string message;
        std::cout << "Escribe un mensaje (o 'salir' para desconectar): ";
        std::getline(std::cin, message);

        std::string messageWithNick = nick + ": " + message;

        send(clientSocket, messageWithNick.c_str(), messageWithNick.length(), 0);
        if (message == "salir") {
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        int valread = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (valread > 0) { // Solo procesar si hay datos
            std::cout << "Servidor: " << buffer << std::endl;
        } else if (valread == 0) {
            std::cout << "El servidor ha cerrado la conexión." << std::endl;
            break;
        } else {
            std::cerr << "Error en la recepción del mensaje." << std::endl;
            break;
        }
    }

    close(clientSocket);
}
