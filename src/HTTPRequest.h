//
// Created by Hugo on 8/30/2018.
//

#ifndef SIMPLE_HTTP_SERVER_HTTPREQUEST_H
#define SIMPLE_HTTP_SERVER_HTTPREQUEST_H

#define REQUEST_METHOD 0
#define REQUEST_URI 1
#define REQUEST_PROTOCOL 2

class HTTPRequest {
private:
    char *rawRequest;
    char **requestParts;
    char **requestLine;
    char ***headers;
    int requestLineCount;
    int headerCount;

    void parse();

public:
    HTTPRequest(char *rawRequest);

    int countRequestLines();

    void printRawRequest();

    void splitRequest();

    void parseRequestLine();

    void parseRequestHeader();

    void parserHeader(int i);

    char *getRequestLine();

    char *getRequestLine(int i);

    char *getRawHeader(int i);

    char *getHeaderValue(int i);

    char *copyString(const char *rawRequest);

    char *copyRawRequest();

    char *getQueryString();

    char *getRequest();

    char *getLine(int line);


    char **getHeader(int i);

    char **getHeaderValue(const char *string);
};


#endif //SIMPLE_HTTP_SERVER_HTTPREQUEST_H
