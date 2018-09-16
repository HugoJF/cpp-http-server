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

    /// \param listener - listener and connection handling class reference
    /// \param httpRequest - request object reference
    Worker(Listener *listener, HTTPRequest *httpRequest);

    /// Solves given HTTP request
    void work();

    char *serveCgiBin(char *fixedUri) const;

    char *serveStaticFile(char *fixedUri) const;
};

#endif //SIMPLE_HTTP_SERVER_WORKER_H