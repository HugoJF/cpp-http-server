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

    if (stat(this->BuildPath(filePath), sb) == -1) {
        perror("ResolveFilePath()");
        return nullptr;
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

int FileRequest::Solve() {

    if(this->filePath == nullptr) {
        return 1; // file not found
    }

    int filefd = open(this->filePath, O_RDONLY);

    if (filefd < 0) {
        perror("open");
        return errno;
    }

    int bufferSize = sizeof(char) * (sb->st_size + 1); // +1 for \0
    char *buffer = new char[bufferSize];
    memset(buffer, '\0', sizeof(char) * bufferSize);

    auto readBytesTotal = (_ssize_t) 0;
    auto readBytes = (_ssize_t) bufferSize;

    while (readBytes >= bufferSize) {
        printf("#### Reading data file...\n");
        readBytes = read(filefd, buffer + readBytesTotal, (size_t) bufferSize);

        if (readBytes < 0) {
            printf("Error reading file: %s\n", strerror(errno));
            return errno;
        } else {
            printf("Read %d bytes\n", (int) readBytes);
            readBytesTotal += readBytes;
        }
    }

    close(filefd);

    this->fileContent = buffer;

    return 0;
}

char *FileRequest::GetResponse() {
    return this->fileContent;
}
