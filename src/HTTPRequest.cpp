//
// Created by Hugo on 8/30/2018.
//

#include "HTTPRequest.h"
#include <iostream>
#include <string.h>

HTTPRequest::HTTPRequest(char *rawRequest) {
    this->rawRequest = CopyString(rawRequest);

    Parse();
}

char *HTTPRequest::CopyString(const char *rawRequest) {
    char *clone = (char *) malloc(sizeof(char) * (strlen(rawRequest) + 1));
    memcpy(clone, rawRequest, sizeof(char) * (strlen(rawRequest) + 1));

    return clone;
}

char *HTTPRequest::CopyRawRequest() {
    return CopyString(this->rawRequest);
}


void HTTPRequest::Parse() {
    int count = CountRequestLines();

    SplitRequest();
    ParseRequestLine();
    ParseRequestHeader();
}

int HTTPRequest::CountRequestLines() {
    // HTTP Request end with 2 \r\n,
    int count = -1;
    char *movingPtr = CopyRawRequest();
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

void HTTPRequest::SplitRequest() {
    char *piece = nullptr;
    char *rawRequest = CopyRawRequest();
    int line = 0;
    piece = strtok(rawRequest, "\r\n");
    while (piece != nullptr) {
        requestParts[line++] = piece;
        piece = strtok(nullptr, "\r\n");
    }
}

char *HTTPRequest::GetRequestLine() {
    return GetLine(0);
}

char *HTTPRequest::GetLine(int line) {
    return requestParts[line];
}


void HTTPRequest::ParseRequestLine() {
    requestLine = new char *[3];
    char *piece = nullptr;
    int part = 0;
    piece = strtok(GetRequestLine(), " ");
    while (piece != nullptr) {
        requestLine[part++] = piece;
        piece = strtok(nullptr, " ");
    }
}

void HTTPRequest::ParseRequestHeader() {
    headers = new char **[requestLineCount - 1];

    for (int i = 0; i < requestLineCount - 1; ++i) {
        ParserHeader(i);
    }
}

char *HTTPRequest::GetRequestLine(int i) {
    return requestLine[i];
}

void HTTPRequest::ParserHeader(int i) {
    char *key;
    char *value;
    key = strtok(GetRawHeader(i), ":");
    value = strtok(nullptr, ":");
    headers[i] = new char *[2];
    headers[i][0] = key;
    headers[i][1] = value;
}

char *HTTPRequest::GetRawHeader(int i) {
    return GetLine(i + 1);
}

char **HTTPRequest::GetHeader(int i) {
    return headers[i];
}

char *HTTPRequest::GetHeaderValue(int i) {
    return headers[i][1];
}

char **HTTPRequest::GetHeaderValue(const char *string) {
    char **head;
    for (int i = 0; i < headerCount; ++i) {
        head = GetHeader(i);
        if (strcmp(string, head[0]) == 0) {
            return head;
        }
    }
    return nullptr;
}

char *HTTPRequest::getQueryString() {
    char *uri = this->GetRequestLine(REQUEST_URI);

    // Get pointer to characters following '?'
    char *start = index(uri, '?') + 1;
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
