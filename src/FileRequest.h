//
// Created by Hugo on 8/31/2018.
//

#ifndef SIMPLE_HTTP_SERVER_FILEREQUEST_H
#define SIMPLE_HTTP_SERVER_FILEREQUEST_H


class FileRequest {
    char *filePath;
    char *fileContent;
    struct stat *sb;

    char *BuildPath(const char*);
    char *ResolveFilePath(char* filePath);
public:
    FileRequest(char* filePath);
    void Solve();
    char *GetResponse();

};


#endif //SIMPLE_HTTP_SERVER_FILEREQUEST_H
