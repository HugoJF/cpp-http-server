//
// Created by Hugo on 8/30/2018.
//

#ifndef SIMPLE_HTTP_SERVER_WORKER_H
#define SIMPLE_HTTP_SERVER_WORKER_H

#include <src/HTTPRequest.h>
#include <src/Listener.h>

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

private:
    /// \param response - full response to send to client
    /// \param fd - what connection to use
    void sendResponseToClient(char *response, int fd);
};

#endif //SIMPLE_HTTP_SERVER_WORKER_H