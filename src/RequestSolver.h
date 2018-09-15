//
// Created by Hugo on 9/1/2018.
//

#ifndef SIMPLE_HTTP_SERVER_REQUESTSOLVER_H
#define SIMPLE_HTTP_SERVER_REQUESTSOLVER_H


class RequestSolver {
private:
    /// Raw request URI
    char *uri;

    /// Split request URI
    char **segments;

    /// How many segments the URI has
    int segmentCount;

    /// Counts how many segments the URI has
    int countSegments();

public:
    /// \param uri - request URI
    RequestSolver(char *uri);

    /// \return - is request for directory (index.html or listing)
    bool isDirectory();

    /// \return - is request for static file
    bool isFile();

    /// \return - is request for cgi-bin script
    bool isCgiBin();

    /// Splits request into multiple segments
    void process();

    /// \return - request segment count
    int getSegmentCount();

    /// \param i - segment index
    /// \return - specified segment
    const char *getSegment(int i);

};


#endif //SIMPLE_HTTP_SERVER_REQUESTSOLVER_H
