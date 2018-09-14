//
// Created by Hugo on 8/30/2018.
//

#ifndef SIMPLE_HTTP_SERVER_LISTENER_H
#define SIMPLE_HTTP_SERVER_LISTENER_H

#include <netinet/in.h>
#include <src/HTTPRequest.h> // HTTPRequest

class Listener {
private:
    int connectionFd = 0;
public:

    Listener(int connectionFd);

    uint16_t getPort() const;

    int getListenFd() const;

    int getConnectionFd() const;

    const sockaddr_in &getServerAddress() const;

    HTTPRequest *readRequest();

    void close();
};


#endif //SIMPLE_HTTP_SERVER_LISTENER_H
