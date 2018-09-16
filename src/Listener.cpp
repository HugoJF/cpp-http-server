//
// Created by Hugo on 8/30/2018.
//

#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "inc/HTTPRequest.h"
#include "inc/Listener.h"

Listener::Listener(int connectionFd) {
    this->connectionFd = connectionFd;
}

HTTPRequest *Listener::readRequest() {
    auto bufferSize = (size_t) 1024;
    char buffer[bufferSize + 1];
    memset(buffer, '\0', bufferSize);
    auto requestBytesTotal =  0;
    auto requestBytes = bufferSize;
    auto request = new std::string();

    while (requestBytes == bufferSize && requestBytesTotal < bufferSize) {
        printf("Reading request... ");
        requestBytes = read(connectionFd, buffer, bufferSize);

        if (requestBytes < 0) {
            printf("ERROR: %s\n", strerror(errno));

            return nullptr;
        } else {
            printf("Read %d bytes\n", (int) requestBytes);
            requestBytesTotal += requestBytes;
            request->append(buffer, requestBytes);
        }
    }

    printf("Request total size: %d bytes.\n", (int) requestBytesTotal);

    if(requestBytesTotal <= 0) {
        return nullptr;
    }

    auto *hp = new HTTPRequest((char*)request->c_str());

    if (hp->getRequestLineCount() == 0) {
        return nullptr;
    }

    printf("Request: %s\n", hp->getRequestLine());

    return hp;

}

void Listener::sendResponse(const char *response) {
    int bytesToSend = sizeof(char) * strlen(response);
    ssize_t bytesSent = 0;

    while (bytesToSend > 0) {
        bytesSent = send(getConnectionFd(), response + (strlen(response) - bytesToSend), sizeof(char) * bytesToSend, 0);
        bytesToSend -= bytesSent;
    }
}

int Listener::getConnectionFd() const {
    return connectionFd;
}

void Listener::close() {
    printf("Closing connection\n");
    ::close(connectionFd);
}
