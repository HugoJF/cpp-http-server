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
    auto *request = new RequestSolver(uri);

    // Split request URI
    request->process();

    // URI with /index.html appended to the end (default behavior)
    char *fixedUri;

    // If request was internally redirected to CGI-BIN
    bool forceCgiBin = false;

    if (request->isDirectory()) {
        // Append index.html to request URI
        fixedUri = new char[strlen(uri) + strlen(INDEX_APPEND) + 1];
        strcpy(fixedUri, uri);
        strncat(fixedUri, INDEX_APPEND, sizeof(INDEX_APPEND));

        // Check if index.html exists and can be opened
        auto *indexRequest = new FileRequest(fixedUri);
        int result = indexRequest->solve();
        delete indexRequest;

        // If index.html cannot be accessed, redirect to cgi-bin directory listing script
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

    if (request->isCgiBin() || forceCgiBin) {
        response = serveCgiBin(fixedUri);
    } else {
        response = serveStaticFile(fixedUri);
    }

    if (response == nullptr || strlen(response) == 0) {
        printf("Could not send correct response to client: empty response. Serving 404 error\n");
        response = serve404(uri);
    }

    printf("Returning response contains %d bytes.\n", responseSize);
    listener->sendResponse(response, responseSize);
}

char *Worker::serveStaticFile(char *fixedUri) {
    printf("Request is for static file.\n");
    auto *fr = new FileRequest(fixedUri);
    auto *rb = new HeaderBuilder();
    string *res = new string();

    int result = fr->solve();

    if(result == 1) {
        return nullptr;
    }

    addContentTypeHeader(fixedUri, rb);
    rb->addHeader("Content-Length", (char *) std::to_string(fr->getFileContentSize()).c_str());

    res->append("HTTP/1.1 200 OK\r\n");
    res->append(rb->render());
    res->append("\r\n");
    responseSize = strlen(res->c_str());
    res->append(fr->getResponse(), (unsigned long) fr->getFileContentSize());

    responseSize += fr->getFileContentSize();

    return (char *) res->c_str();
}

void Worker::addContentTypeHeader(const char *fixedUri, HeaderBuilder *rb) const {
    if (strstr(fixedUri, "jpeg") != nullptr || strstr(fixedUri, "jpg") != nullptr) {
        rb->addHeader("Content-Type", "image/jpeg");
    } else if (strstr(fixedUri, "png")) {
        rb->addHeader("Content-Type", "image/png");
    } else if (strstr(fixedUri, "html") || strstr(fixedUri, "css")) {
        rb->addHeader("Content-Type", "text/html");
    } else if (strstr(fixedUri, "svg")) {
        rb->addHeader("Content-Type", "image/svg+xml");
    } else {
        rb->addHeader("Content-Type", "text/plain");
    }
}

char *Worker::serveCgiBin(char *fixedUri) {
    printf("Request is inside CGI-BIN directory.\n");
    auto *cgi = new CgiBinRequest(fixedUri, httpRequest);

    cgi->addEnvironmentVariables(("PATH_INFO"), httpRequest->getRequestLine(REQUEST_URI));
    cgi->addEnvironmentVariables("QUERY_STRING", httpRequest->getQueryString());
    cgi->solve();

    responseSize = (int) strlen(cgi->getResponse());

    return cgi->getResponse();
}

char *Worker::serve404(char *fixedUri) {
    string *res = new string();

    res->append("HTTP/1.1 404 Not Found\r\n");
    res->append("Content-Type: text/plain\r\n\r\n");
    res->append("The requested URL ");
    res->append(fixedUri);
    res->append(" could not be found on this server.");

    responseSize = res->size();

    return (char *) res->c_str();
}
