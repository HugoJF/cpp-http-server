//
// Created by Hugo on 8/30/2018.
//

#ifndef SIMPLE_HTTP_SERVER_WORKER_H
#define SIMPLE_HTTP_SERVER_WORKER_H

#include "HTTPRequest.h"
#include "Listener.h"

class Worker {
public:
    /// Which request this worker is handling
    HTTPRequest *httpRequest;

    /// Connection handling class
    Listener *listener;

    /// The response string
    char *response;

    /// How many bytes the response is
    int responseSize;

    /// \param listener - listener and connection handling class reference
    /// \param httpRequest - request object reference
    Worker(Listener *listener, HTTPRequest *httpRequest);

    /// Solves given HTTP request
    void work();

    char *serveCgiBin(char *fixedUri);

    char *serveStaticFile(char *fixedUri);

    char *serve404(char *fixedUri);

    void addContentTypeHeader(const char *fixedUri, HeaderBuilder *rb) const;
};

#endif //SIMPLE_HTTP_SERVER_WORKER_H