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
    HTTPRequest *httpRequest;
    int link[2];
    char *buffer;
    string filePath;
    string *response;
    char **argv;
    char **baseEnvironment;
    vector<string> *extendedEnvironment;

    void listen();

    void run();
public:
    CgiBinRequest(string filePath, HTTPRequest *httpRequest);
    void solve();
    char *getResponse();

    void setEnvironmentVariables(char **envp);

    char **getEnvironment();

    char *getFilePath();

    void addEnvironmentVariable(char string[13], char *string1);
};


#endif //SIMPLE_HTTP_SERVER_CGIBINREQUEST_H
