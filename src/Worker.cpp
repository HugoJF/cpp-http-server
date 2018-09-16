//
// Created by Hugo on 8/30/2018.
//

#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "Worker.h"
#include "FileRequest.h"
#include "RequestSolver.h"
#include "CgiBinRequest.h"

#define BUFFER_SIZE 5000
#define INDEX_APPEND "/index.html"

Worker::Worker(Listener *listener, HTTPRequest *httpRequest) {
    this->httpRequest = httpRequest;
    this->listener = listener;
}

void Worker::work() {
    char *uri = httpRequest->getRequestLine(REQUEST_URI);
    auto *rs = new RequestSolver(uri);
    char *response;

    // Split request URI
    rs->process();

    // URI with /index.html appended to the end (default behavior)
    char *fixedUri;

    // If request was internally redirected to CGI-BIN
    bool forceCgiBin = false;

    if (rs->isDirectory()) {
        // TODO: check for trailing slash to avoid // or no slash
        // append index.html to request if file exists
        fixedUri = new char[strlen(uri) + strlen(INDEX_APPEND)];
        sprintf(fixedUri, "%s%s", uri, INDEX_APPEND);

        // check for index.html
        auto *indexRequest = new FileRequest(fixedUri);
        int result = indexRequest->solve();
        delete indexRequest;

        if (result != 0) {
            delete fixedUri;
            fixedUri = new char[strlen("/cgi-bin/dir.py")];
            sprintf(fixedUri, "%s", "/cgi-bin/dir.py");
            printf("Could not find index.html, directing to dir.py to list directory.\n");
            forceCgiBin = true;
        }


    } else {
        // If this is not a directory, the correct URI is the original URI
        fixedUri = uri;
    }

    if (rs->isCgiBin() || forceCgiBin) {
        printf("Request is inside CGI-BIN directory.\n");
        auto *cgi = new CgiBinRequest(fixedUri, httpRequest);

        cgi->addEnvironmentVariableS(("PATH_INFO"), httpRequest->getRequestLine(REQUEST_URI));
        cgi->addEnvironmentVariableS("QUERY_STRING", httpRequest->getQueryString());
        cgi->solve();

        response = cgi->getResponse();
    } else {
        printf("Request is for static file.\n");
        auto *fr = new FileRequest(fixedUri);

        fr->solve();

        response = fr->getResponse();
    }

    if (response == nullptr) {
        printf("Could not send response to client: NULL response\n");
    } else {
        printf("Returning response contains %d bytes.\n", (int) strlen(response));
        sendResponseToClient(response, listener->getConnectionFd());
    }

    delete rs;
}


void Worker::sendResponseToClient(char *response, int fd) {
    int bytesToSend = sizeof(char) * strlen(response);
    ssize_t bytesSent = 0;

    while (bytesToSend > 0) {
        bytesSent = send(fd, response + (strlen(response) - bytesToSend), sizeof(char) * bytesToSend, 0);
        bytesToSend -= bytesSent;
    }
}
