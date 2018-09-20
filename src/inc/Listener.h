//
// Created by Hugo on 8/30/2018.
//

#ifndef SIMPLE_HTTP_SERVER_LISTENER_H
#define SIMPLE_HTTP_SERVER_LISTENER_H

#include <netinet/in.h>
#include "HTTPRequest.h" // HTTPRequest

class Listener {
private:
    int connectionFd = 0;

public:
    /// \param connectionFd - descriptor of connection to listen
    Listener(int connectionFd);

    /// \return - connection file descriptor used in this listener
    int getConnectionFd() const;

    /// \return - HTTPRequest object representing current request received
    HTTPRequest *readRequest();

    /// \param response - the full HTTP response to be sent back to user
    void sendResponse(const char *response, int responseSize);

    /// Closes connection
    void close();
};


#endif //SIMPLE_HTTP_SERVER_LISTENER_H
