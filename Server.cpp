 
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define IP_PORT "127.0.0.3"

constexpr int SERVER_PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

int main() {
    // Создание сокета
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "ERRORS\n";
        return 1;
    }

    // Настройка адреса сервера
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(IP_PORT);

    // Установка соединения с сервером
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "ERRORS\\n";
        close(clientSocket);
        return 1;
    }

    // Отправка сообщения серверу
    const char *message = "Hello, world";
    int bytesSent = send(clientSocket, message, strlen(message), 0);
    if (bytesSent == -1) {
        std::cerr << "ERRORS\n";
        close(clientSocket);
        return 1;
    }

    std::cout << "The message has been sent\n";

    // Получение подтверждения от сервера
    char buffer[BUFFER_SIZE];
    int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesRead == -1) {
        std::cerr << "ERRORS\n";
        close(clientSocket);
        return 1;
    }

    buffer[bytesRead] = '\0';
    std::cout << "Received sms" << buffer << std::endl;

    // Закрытие клиентского сокета
    close(clientSocket);

    return 0;
}
