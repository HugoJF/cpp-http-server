//
// Created by Hugo on 9/1/2018.
//

#include "inc/RequestSolver.h"
#include <string.h>

RequestSolver::RequestSolver(char *uri) {
    this->uri = uri;

    this->process();
}

bool RequestSolver::isDirectory() {
    return !isFile();
}

bool RequestSolver::isFile() {
    return index(getSegment(getSegmentCount() - 1), '.') != nullptr;
}

bool RequestSolver::isCgiBin() {
    return strstr(uri, "cgi-bin");
}


void RequestSolver::process() {
    char *copy = new char[strlen(uri) + 1];
    this->segmentCount = countSegments();
    char **segments = new char *[this->segmentCount];
    strcpy(copy, this->uri);

    char *piece;
    int i = -1;

    while ((piece = strsep(&copy, "/")) != nullptr) {
        if (i == -1) {
            i = 0;
            continue;
        }
        segments[i++] = piece;
    }

    this->segments = segments;
}

int RequestSolver::countSegments() {
    char *ptr = this->uri;
    int count = 0;

    while ((ptr = strstr(ptr, "/"))) {
        count++;
        ptr++;
    }

    return count;
}

int RequestSolver::getSegmentCount() {
    return segmentCount;
}

const char *RequestSolver::getSegment(int i) {
    return segments[i];
}
