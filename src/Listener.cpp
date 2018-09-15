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


HTTPRequest *Listener::readRequest() {

    // TODO: use strings to avoid manually handling buffer
    // TODO: continuous reading to avoid overflowing big requests
    auto bufferSize = (size_t) 1024;
    char buffer[bufferSize + 1];
    memset(buffer, '\0', bufferSize);
    auto requestBytesTotal = (_ssize_t) 0;
    auto requestBytes = (_ssize_t) bufferSize;

    while (requestBytes == bufferSize && requestBytesTotal < bufferSize) {
        printf("Reading request... ");
        requestBytes = read(connectionFd, buffer, bufferSize);

        if (requestBytes < 0) {
            printf("ERROR: %s\n", strerror(errno));

            return nullptr;
        } else {
            printf("Read %d bytes\n", (int) requestBytes);
            requestBytesTotal += requestBytes;
        }
    }

    printf("Request total size: %d bytes.\n", (int) requestBytesTotal);

    auto *hp = new HTTPRequest(buffer);

    if (hp->getRequestLineCount() == 0) {
        return nullptr;
    }

    printf("Request: %s\n", hp->getRequestLine());

    return hp;

}

int Listener::getConnectionFd() const {
    return connectionFd;
}

void Listener::close() {
    // Avoids ERR_CONNECTION_RESET on Chrome
    printf("Closing connection\n");
    ::close(connectionFd);
}
