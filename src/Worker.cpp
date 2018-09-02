//
// Created by Hugo on 8/30/2018.
//

#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <src/Worker.h>
#include <src/FileRequest.h>
#include <src/RequestSolver.h>

#define BUFFER_SIZE 5000

Worker::Worker(Listener *listener, HTTPRequest *httpRequest) {
    this->httpRequest = httpRequest;
    this->listener = listener;
}

void Worker::Work() {

    char *uri = httpRequest->GetRequestLine(REQUEST_URI);
    auto *rs = new RequestSolver(uri);
    rs->process();

    char *fixedUri;
    if(rs->isDirectory()) {
        fixedUri = new char[strlen(uri) + strlen("/index.html")];
        sprintf(fixedUri, "%s%s", uri, "/index.html");
    } else {
        fixedUri = uri;
    }

    auto *fr = new FileRequest(fixedUri);

    fr->Solve();

    SendResponseToClient(fr->GetResponse(), listener->getConnectionFd());
}


void Worker::SendResponseToClient(char *response, int fd) {

    int bytesToSend = sizeof(char) * strlen(response);
    ssize_t bytesSent = 0;

    while (bytesToSend > 0) {
        bytesSent = send(fd, response + (strlen(response) - bytesToSend), sizeof(char) * bytesToSend , 0);
        bytesToSend -= bytesSent;
    }
}
