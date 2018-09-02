//
// Created by Hugo on 8/31/2018.
//

#include "FileRequest.h"
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define HTDOCS_PATH "../htdocs"

FileRequest::FileRequest(char *filePath) {
    sb = new struct stat;
    this->filePath = this->ResolveFilePath(filePath);
}

char *FileRequest::ResolveFilePath(char *filePath) {
    char *fullPath = BuildPath(filePath);

    printf("Full path for file: %s\n", fullPath);

    if (stat(fullPath, sb) == -1) {
        perror("ResolveFilePath()");
        if (errno == 2) { // TODO: better error handling
            fullPath = ResolveFilePath("/404.html");
            printf("File could not be found, replacing with 404 error: %s\n", fullPath);
        } else {
            exit(errno);
        }
    }
    if (sb != nullptr) {
        printf("L Stated File\n");
        printf("File total bytes: %d\n", (int) sb->st_size);
    }

    return fullPath;
}

char *FileRequest::BuildPath(const char *string) {
    auto *fullPath = new char[strlen(HTDOCS_PATH) + strlen(string) + 1];

    sprintf(fullPath, "%s%s", HTDOCS_PATH, string);

    return fullPath;
}

void FileRequest::Solve() {

    int filefd = open(this->filePath, O_RDONLY);

    if (filefd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int bufferSize = sizeof(char) * (sb->st_size + 1); // +1 for \0
    char *buffer = new char[bufferSize];
    memset(buffer, '\0', sizeof(char) * bufferSize);

    auto readBytesTotal = (_ssize_t) 0;
    auto readBytes = (_ssize_t) bufferSize;

    // TODO improve loop
    while (readBytes >= bufferSize) {
        printf("#### Reading data file...\n");
        readBytes = read(filefd, buffer + readBytesTotal, (size_t) bufferSize);

        if (readBytes < 0) {
            printf("Error reading file: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        } else {
            printf("Read %d bytes\n", (int) readBytes);
            readBytesTotal += readBytes;
        }
    }

    this->fileContent = buffer;
}

char *FileRequest::GetResponse() {
    return this->fileContent;
}
