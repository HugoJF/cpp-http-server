//
// Created by Hugo on 8/30/2018.
//

#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <src/HTTPRequest.h>
#include <src/Listener.h>

Listener::Listener() {
}

void Listener::Boot() {
    memset(&this->serverAddress, '0', sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(port);
}

void Listener::BuildSocket() {
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
}

void Listener::Bind() {
    if (bind(listenFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        perror("Bind()");
        exit(EXIT_FAILURE);
    }
}

void Listener::Listen() {
    if (listen(listenFd, 3)) {
        perror("Listen()");
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d\n", port);
}

int Listener::Accept() {
    connectionFd = accept(listenFd, (struct sockaddr *) NULL, NULL);
    if (connectionFd == -1) {
        perror("Accept()");
        exit(EXIT_FAILURE);
    }
}

HTTPRequest *Listener::ReadRequest() {

    auto bufferSize = (_ssize_t) 1024;
    char buffer[bufferSize + 1];
    memset(buffer, '\0', bufferSize);
    auto requestBytesTotal = (_ssize_t) 0;
    auto requestBytes = (_ssize_t) bufferSize;

    while (requestBytes == bufferSize && requestBytesTotal < bufferSize) {
        printf("Waiting for bytes...\n");
        requestBytes = read(connectionFd, buffer, (size_t) bufferSize);
        printf("Received bytes!\n");

        if (requestBytes < 0) {
            printf("Error reading message: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        } else {
            printf("Read %d bytes: %s\n", (int) requestBytes, buffer);
            requestBytesTotal += requestBytes;
        }
    }

    auto *hp = new HTTPRequest(buffer);

    printf("Request method: %s\n", hp->GetRequestLine(REQUEST_METHOD));
    printf("Request URI: %s\n", hp->GetRequestLine(REQUEST_URI));
    printf("Request protocol: %s\n", hp->GetRequestLine(REQUEST_PROTOCOL));

    return hp;

}

uint16_t Listener::getPort() const {
    return port;
}

int Listener::getListenFd() const {
    return listenFd;
}

int Listener::getConnectionFd() const {
    return connectionFd;
}

const sockaddr_in &Listener::getServerAddress() const {
    return serverAddress;
}

void Listener::Close() {
    // Avoids ERR_CONNECTION_RESET on Chrome
    printf("Closing connection\n");
    close(connectionFd);
}
