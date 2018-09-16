//
// Created by Hugo on 8/30/2018.
//

#include "inc/HTTPRequest.h"
#include "inc/HeaderBuilder.h"
#include <iostream>
#include <string.h>

HTTPRequest::HTTPRequest(char *rawRequest) {
    this->rawRequest = copyString(rawRequest);
    this->headers = new HeaderBuilder();
    requestLine = new char *[3];

    parse();
}

char *HTTPRequest::copyString(const char *string) {
    char *clone = new char[strlen(string) + 1];

    strcpy(clone, string);

    return clone;
}

char *HTTPRequest::copyRawRequest() {
    return copyString(rawRequest);
}

void HTTPRequest::parse() {
    int count = countRequestLines();

    printf("Parsed %d request lines.\n", count);

    if (count > 0) {
        splitRequest();
        parseRequestLine();
        parseRequestHeader();
    }
}

int HTTPRequest::countRequestLines() {
    // HTTP Request end with 2 \r\n,
    int count = -1;
    char *movingPtr = copyRawRequest();
    while ((movingPtr = strstr(movingPtr, "\r\n"))) {
        count++;
        movingPtr++;
    }
    if (count > 0) {
        requestLineCount = count;
        requestParts = new char *[count];
        // clonar strings
        return count;
    } else {
        return 0;
    }
}

int HTTPRequest::getRequestLineCount() const {
    return requestLineCount;
}

void HTTPRequest::splitRequest() {
    char *piece = nullptr;
    char *rawRequest = copyRawRequest();
    int line = 0;
    piece = strtok(rawRequest, "\r\n");
    while (piece != nullptr) {
        requestParts[line++] = piece;
        piece = strtok(nullptr, "\r\n");
    }
}

char *HTTPRequest::getRequestLine() {
    return getLine(0);
}

char *HTTPRequest::getLine(int line) {
    return requestParts[line];
}


void HTTPRequest::parseRequestLine() {
    char *piece = nullptr;
    char *request = new char[strlen(getRequestLine()) + 1];
    strcpy(request, getRequestLine());

    requestLine[0] = strtok(request, " ");
    requestLine[1] = strtok(nullptr, " ");
    requestLine[2] = strtok(nullptr, " ");

}

void HTTPRequest::parseRequestHeader() {
    for (int i = 0; i < requestLineCount - 1; ++i) {
        parserHeader(i);
    }
}

char *HTTPRequest::getRequestLine(int i) {
    return requestLine[i];
}

void HTTPRequest::parserHeader(int i) {
    char *header = new char[strlen(getRawHeader(i))];

    strcpy(header, getRawHeader(i));

    char *key = strtok(header, ":");
    char *value = strtok(nullptr, ":");

    this->getHeaderBuilder()->addHeader(key, value);
}

HeaderBuilder *HTTPRequest::getHeaderBuilder() const {
    return headers;
}

char *HTTPRequest::getRawHeader(int i) {
    return getLine(i + 1);
}


char *HTTPRequest::getHeaderValue(int i) {
    return this->getHeaderBuilder()->getHeader(i)->value;
}

char *HTTPRequest::getQueryString() {
    char *uri = this->getRequestLine(REQUEST_URI);

    // Get pointer to characters after '?'
    char *start = index(uri, '?');

    // If there are no parameters point to end of string (no query string)
    if (start == nullptr) {
        start = uri + strlen(uri);
    } else {
        start += 1;
    }
    // Get pointer to end of string
    char *end = (uri + strlen(uri));
    // Calculate how many chars to the end of string + null terminator
    int delta = (int) (end - start) + 1;
    // Allocate string
    char *result = new char[delta];
    // Copy piece of memory
    memcpy(result, start, (size_t) delta);

    return result;
}
