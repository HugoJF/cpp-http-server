//
// Created by Hugo on 9/1/2018.
//

#ifndef SIMPLE_HTTP_SERVER_CGIBINREQUEST_H
#define SIMPLE_HTTP_SERVER_CGIBINREQUEST_H

#include <src/HTTPRequest.h>
#include <vector>
#include <string>

using namespace std;

class CgiBinRequest {
private:
    // HTTP request that generated this CGI-BIN request
    HTTPRequest *httpRequest;

    // File descriptors for execv stdout pipe
    int link[2];

    // Generic buffer (mainly used while reading stdout from execv)
    char *buffer;

    // Relative path to CGI script
    string *filePath;

    // Full response from CGI script
    string *response;

    // Arguments pointer passed to CGI script
    char **argv;

    // Vector containing extra environment variables passed to CGI script
    vector<string> *extendedEnvironment;

    // Listens for child process output
    void listen();

    // Runs CGI script in forked process
    void run();

public:
    ///
    /// \param filePath - Script path
    /// \param httpRequest - HTTP request for this CGI
    CgiBinRequest(char *filePath, HTTPRequest *httpRequest);

    /// Executes script and solves request
    void solve();

    /// \return - Response from CGI script
    char *getResponse();

    /// \return - Relative path to script file
    char *getFilePath();

    /// \return - environment variables with extended environment
    char **getEnvironment();

    /// Adds new environment variable to CGI script
    /// \param key - variable key
    /// \param value - variable value
    void addEnvironmentVariable(char *key, char *value);

    /// Adds new environment variable to CGI script (this is only used to workaround stupid C++ string/char* confusion)
    /// \param key - variable key
    /// \param value - variable value
    void addEnvironmentVariableS(string key, char *value);

    /// Counts base environment variables
    int getBaseEnvironmentCount() const;

    /// Counts base + extended environment variables
    int getTotalEnvironmentCount() const;

    /// Prints environment variables to screen
    void debugEnvironmentVariables();

    /// Creates new pipe for parent-child process communication
    void createPipe();
};


#endif //SIMPLE_HTTP_SERVER_CGIBINREQUEST_H
