//
// Created by Hugo on 8/30/2018.
//

#ifndef SIMPLE_HTTP_SERVER_LISTENER_H
#define SIMPLE_HTTP_SERVER_LISTENER_H

#include <netinet/in.h>
#include <src/HTTPRequest.h> // HTTPRequest

class Listener {
private:
    uint16_t port = 8080;
    int listenFd = 0;
    int connectionFd = 0;
    struct sockaddr_in serverAddress;
public:
    uint16_t getPort() const;

    int getListenFd() const;

    int getConnectionFd() const;

    const sockaddr_in &getServerAddress() const;

    Listener();

    void Boot();

    void BuildSocket();

    void Bind();

    void Listen();

    int Accept();

    HTTPRequest *ReadRequest();

    void Close();
};


#endif //SIMPLE_HTTP_SERVER_LISTENER_H
