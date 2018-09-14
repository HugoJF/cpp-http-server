//
// Created by Hugo on 8/31/2018.
//

#ifndef SIMPLE_HTTP_SERVER_FILEREQUEST_H
#define SIMPLE_HTTP_SERVER_FILEREQUEST_H


class FileRequest {
    char *filePath;
    char *fileContent;
    struct stat *sb;

    char *buildPath(const char *);

    char *resolveFilePath(char *filePath);

public:
    FileRequest(char *filePath);

    int solve();

    char *getResponse();

};


#endif //SIMPLE_HTTP_SERVER_FILEREQUEST_H
