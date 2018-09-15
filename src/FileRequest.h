//
// Created by Hugo on 8/31/2018.
//

#ifndef SIMPLE_HTTP_SERVER_FILEREQUEST_H
#define SIMPLE_HTTP_SERVER_FILEREQUEST_H


class FileRequest {
    /// Relative path to file
    char *filePath;

    /// File contents
    char *fileContent;

    /// File stat structure
    struct stat *sb;

    /// Prefix path to point to htdocs folder
    char *buildPath(const char *);

    /// Checks if built path is valid and file exists
    char *resolveFilePath(char *filePath);

public:
    /// \param filePath - Relative path to file
    FileRequest(char *filePath);

    /// Builds path, check file if file exists and read entire file contents
    /// \return - 0 on success, 1 if not found, error code on error
    int solve();

    /// \return - Cached file contents
    char *getResponse();

};


#endif //SIMPLE_HTTP_SERVER_FILEREQUEST_H
