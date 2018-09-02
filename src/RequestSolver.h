//
// Created by Hugo on 9/1/2018.
//

#ifndef SIMPLE_HTTP_SERVER_REQUESTSOLVER_H
#define SIMPLE_HTTP_SERVER_REQUESTSOLVER_H


class RequestSolver {
private:
    char *uri;
    char **segments;
    int segmentCount;

    int countSegments();

public:
    RequestSolver(char *uri);

    bool isDirectory();
    bool isFile();
    bool isCgiBin();

    void process();

    int getSegmentCount();

    const char *getSegment(int i);

};


#endif //SIMPLE_HTTP_SERVER_REQUESTSOLVER_H
