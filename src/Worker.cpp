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
#include <src/CgiBinRequest.h>

#define BUFFER_SIZE 5000

Worker::Worker(Listener *listener, HTTPRequest *httpRequest) {
    this->httpRequest = httpRequest;
    this->listener = listener;
}

void Worker::Work() {
    char *uri = httpRequest->GetRequestLine(REQUEST_URI);
    auto *rs = new RequestSolver(uri);
    char *response;
    rs->process();

    // URI with /index.html appended to the end (default behavior)
    char *fixedUri;
    bool forceCgiBin = false;

    if (rs->isDirectory()) {
        // fix URL if index.html exists
        // TODO: check for trailing slash to avoid // or no slash
        fixedUri = new char[strlen(uri) + strlen("/index.html")];
        sprintf(fixedUri, "%s%s", uri, "/index.html");

        // check for index.html
        auto *indexRequest = new FileRequest(fixedUri);
        int result = indexRequest->Solve();

        if (result != 0) {
            delete fixedUri;
            fixedUri = new char[strlen("/cgi-bin/dir.py")];
            sprintf(fixedUri, "%s", "/cgi-bin/dir.py");
            printf("Could not find index.html, directing to dir.py to list directory...\n");
            forceCgiBin = true;
        }


    } else {
        // If this is not a directory, the correct URI is the original URI
        fixedUri = uri;

    }
        if (rs->isCgiBin() || forceCgiBin) {
            printf("Request is inside CGI-BIN directory...\n");
            auto *cgi = new CgiBinRequest(fixedUri, httpRequest);
            cgi->setEnvironmentVariables(environ);
            cgi->addEnvironmentVariable("PATH_INFO", httpRequest->GetRequestLine(REQUEST_URI));
            cgi->addEnvironmentVariable("QUERY_STRING", httpRequest->getQueryString());
            cgi->solve();
            response = cgi->getResponse();
        } else {
            printf("Request is for static file\n");
            auto *fr = new FileRequest(fixedUri);
            fr->Solve();
            response = fr->GetResponse();
        }

    printf("Returning response: (%s)\n", response);

    if(response == nullptr) {
        printf("Could not send response to client: NULL response\n");
    } else {
        SendResponseToClient(response, listener->getConnectionFd());
    }
}


void Worker::SendResponseToClient(char *response, int fd) {

    int bytesToSend = sizeof(char) * strlen(response);
    ssize_t bytesSent = 0;

    while (bytesToSend > 0) {
        bytesSent = send(fd, response + (strlen(response) - bytesToSend), sizeof(char) * bytesToSend, 0);
        bytesToSend -= bytesSent;
    }
}
