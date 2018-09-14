//
// Created by Hugo on 8/30/2018.
//

#include "HTTPRequest.h"
#include <iostream>
#include <string.h>

HTTPRequest::HTTPRequest(char *rawRequest) {
    this->rawRequest = copyString(rawRequest);

    parse();
}

char *HTTPRequest::copyString(const char *rawRequest) {
    char *clone = (char *) malloc(sizeof(char) * (strlen(rawRequest) + 1));
    memcpy(clone, rawRequest, sizeof(char) * (strlen(rawRequest) + 1));

    return clone;
}

char *HTTPRequest::copyRawRequest() {
    return copyString(this->rawRequest);
}


void HTTPRequest::parse() {
    int count = countRequestLines();

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
        printf("Request line count: %d\n", count);
        requestLineCount = count;
        headerCount = count - 1;
        requestParts = new char *[count];
        // clonar strings
        return count;
    } else {
        return 0;
    }
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
    requestLine = new char *[3];
    char *piece = nullptr;
    int part = 0;
    piece = strtok(getRequestLine(), " ");
    while (piece != nullptr) {
        requestLine[part++] = piece;
        piece = strtok(nullptr, " ");
    }
}

void HTTPRequest::parseRequestHeader() {
    headers = new char **[requestLineCount - 1];

    for (int i = 0; i < requestLineCount - 1; ++i) {
        parserHeader(i);
    }
}

char *HTTPRequest::getRequestLine(int i) {
    return requestLine[i];
}

void HTTPRequest::parserHeader(int i) {
    char *key;
    char *value;
    key = strtok(getRawHeader(i), ":");
    value = strtok(nullptr, ":");
    headers[i] = new char *[2];
    headers[i][0] = key;
    headers[i][1] = value;
}

char *HTTPRequest::getRawHeader(int i) {
    return getLine(i + 1);
}

char **HTTPRequest::getHeader(int i) {
    return headers[i];
}

char *HTTPRequest::getHeaderValue(int i) {
    return headers[i][1];
}

char **HTTPRequest::getHeaderValue(const char *string) {
    char **head;
    for (int i = 0; i < headerCount; ++i) {
        head = getHeader(i);
        if (strcmp(string, head[0]) == 0) {
            return head;
        }
    }
    return nullptr;
}

char *HTTPRequest::getQueryString() {
    char *uri = this->getRequestLine(REQUEST_URI);

    // Get pointer to characters following '?'
    char *start = index(uri, '?');

    // If there are no parameters
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
