//
// Created by Hugo on 8/30/2018.
//

#ifndef SIMPLE_HTTP_SERVER_WORKER_H
#define SIMPLE_HTTP_SERVER_WORKER_H

#include <src/HTTPRequest.h>
#include <src/Listener.h>

class Worker {
public:
    HTTPRequest *httpRequest;
    Listener *listener;

    Worker(Listener *listener, HTTPRequest *httpRequest);

    void work();

private:
    void sendResponseToClient(char *path, int fd);
};


#endif //SIMPLE_HTTP_SERVER_WORKER_H
