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

Listener::Listener(int connectionFd) {
    this->connectionFd = connectionFd;
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
            printf("Read %d bytes\n", (int) requestBytes);
            requestBytesTotal += requestBytes;
        }
    }

    printf("Entire request was %d bytes.\n", (int) requestBytesTotal);

    auto *hp = new HTTPRequest(buffer);

    printf("Request method: %s\n", hp->GetRequestLine(REQUEST_METHOD));
    printf("Request URI: %s\n", hp->GetRequestLine(REQUEST_URI));
    printf("Request protocol: %s\n", hp->GetRequestLine(REQUEST_PROTOCOL));

    return hp;

}

int Listener::getConnectionFd() const {
    return connectionFd;
}

void Listener::Close() {
    // Avoids ERR_CONNECTION_RESET on Chrome
    printf("Closing connection\n");
    close(connectionFd);
}
