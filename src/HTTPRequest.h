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

    void Parse();

public:
    HTTPRequest(char *rawRequest);

    void PrintRawRequest();

    char *GetRequest();

    char *GetLine(int line);

    int CountRequestLines();

    void SplitRequest();

    void ParseRequestLine();

    void ParseRequestHeader();

    char *GetRequestLine();

    char *GetRequestLine(int i);

    void ParserHeader(int i);

    char *GetRawHeader(int i);

    char **GetHeader(int i);

    char *GetHeaderValue(int i);

    char **GetHeaderValue(const char string[7]);


    char* CopyString(const char *rawRequest);

    char *CopyRawRequest();

    char *getQueryString();
};


#endif //SIMPLE_HTTP_SERVER_HTTPREQUEST_H
