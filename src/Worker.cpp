//
// Created by Hugo on 8/30/2018.
//

#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "inc/Worker.h"
#include "inc/FileRequest.h"
#include "inc/RequestSolver.h"
#include "inc/CgiBinRequest.h"
#include "inc/HeaderBuilder.h"

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
        fixedUri = new char[strlen(uri) + strlen(INDEX_APPEND) + 1];
        strcpy(fixedUri, uri);
        strncat(fixedUri, INDEX_APPEND, sizeof(INDEX_APPEND));

        // check for index.html
        auto *indexRequest = new FileRequest(fixedUri);
        int result = indexRequest->solve();
        delete indexRequest;

        if (result != 0) {
            delete fixedUri;
            fixedUri = new char[strlen("/cgi-bin/dir.py") + 1];
            sprintf(fixedUri, "%s", "/cgi-bin/dir.py");
            printf("Could not find index.html, directing to dir.py to list directory.\n");
            forceCgiBin = true;
        }


    } else {
        // If this is not a directory, the correct URI is the original URI
        fixedUri = uri;
    }

    if (rs->isCgiBin() || forceCgiBin) {
        response = serveCgiBin(fixedUri);
    } else {
        response = serveStaticFile(fixedUri);
    }

    if (response == nullptr) {
        printf("Could not send response to client: NULL response\n");
    } else {
        printf("Returning response contains %d bytes.\n", (int) strlen(response));
        listener->sendResponse(response);
    }

    delete response;
    delete rs;
}

char *Worker::serveStaticFile(char *fixedUri) const {
    char *response;
    printf("Request is for static file.\n");
    auto *fr = new FileRequest(fixedUri);
    auto *rb = new HeaderBuilder();
    string *res = new string();

    char *fileContent = fr->getResponse();

    fr->solve();
    rb->addHeader("Content-Type", "text/html");
    rb->addHeader("Content-Length", (char*) std::to_string(strlen(fileContent)).c_str());

    char *headers = rb->render();

    res->append("HTTP/1.1 200 OK");
    res->append(rb->render());
    res->append("\r\n");
    res->append(fr->getResponse());

    response = (char *) res->c_str();
    return response;
}

char *Worker::serveCgiBin(char *fixedUri) const {
    char *response;
    printf("Request is inside CGI-BIN directory.\n");
    auto *cgi = new CgiBinRequest(fixedUri, httpRequest);

    cgi->addEnvironmentVariables(("PATH_INFO"), httpRequest->getRequestLine(REQUEST_URI));
    cgi->addEnvironmentVariables("QUERY_STRING", httpRequest->getQueryString());
    cgi->solve();

    response = cgi->getResponse();
    return response;
}