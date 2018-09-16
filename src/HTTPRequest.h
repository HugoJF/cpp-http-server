//
// Created by Hugo on 8/30/2018.
//

#ifndef SIMPLE_HTTP_SERVER_HTTPREQUEST_H
#define SIMPLE_HTTP_SERVER_HTTPREQUEST_H

#include <src/HeaderBuilder.h>

#define REQUEST_METHOD 0
#define REQUEST_URI 1
#define REQUEST_PROTOCOL 2

class HTTPRequest {
private:
    /// Raw HTTP request string
    char *rawRequest;

    /// Each line of the request
    char **requestParts;

    /// 3 segments of the main request line
    char **requestLine;

    /// Array of request headers
    HeaderBuilder *headers;

    /// How many lines the request has
    int requestLineCount;

    /// Parses the raw request
    void parse();

public:
    /// \param rawRequest - request string
    HTTPRequest(char *rawRequest);

    /// Count new lines in request string
    /// \return - request line count
    int countRequestLines();

    /// Splits each line of the request
    void splitRequest();

    /// Splits method, uri and protocol from request main line
    void parseRequestLine();

    /// Parses remaining request (headers)
    void parseRequestHeader();

    /// Parses specific header
    /// \param i - header index
    void parserHeader(int i);

    /// \return - returns main request line
    char *getRequestLine();

    /// \param i - request line index
    /// \return - request line string
    char *getRequestLine(int i);

    /// \param i - header index
    /// \return - raw header with index i
    char *getRawHeader(int i);

    /// \param i - index of header
    /// \return - header value
    char *getHeaderValue(int i);

    /// \param string - string to copy
    /// \return - new string copy
    char *copyString(const char *string);

    /// \return - raw request copy
    char *copyRawRequest();

    /// \return - URI content after '?'
    char *getQueryString();

    /// \param line - line index
    /// \return - raw request line
    char *getLine(int line);

    /// \return - request line count
    int getRequestLineCount() const;

    HeaderBuilder *getHeaderBuilder() const;
};


#endif //SIMPLE_HTTP_SERVER_HTTPREQUEST_H
