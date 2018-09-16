

//
// Created by Hugo on 9/1/2018.
//
#include <utility>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string.h>
#include <cerrno>
#include "inc/CgiBinRequest.h"
#include "inc/HTTPRequest.h"

#define BUFFER_SIZE 1024
#define CGI_BIN_FOLDER "/home/hugo/cpp-http-server"
// TODO: Avoid using fixed buffer, if response is bigger than BUFFER_SIZE overflow will occur

CgiBinRequest::CgiBinRequest(char *filePath, HTTPRequest *httpRequest) {
    this->httpRequest = httpRequest;
    this->buffer = new char[BUFFER_SIZE + 1];
    this->extendedEnvironment = new vector<string>;
    this->argv = new char *[1];
    this->filePath = new string(filePath);
    this->response = new string();

    argv[0] = nullptr;

    char *f = this->getFilePath();
    printf("CGI-BIN Path: %s\n", f);
    delete f;
}

CgiBinRequest::~CgiBinRequest() {
    delete[] this->buffer;
    delete[] this->argv;
    this->extendedEnvironment->clear();
}

void CgiBinRequest::run() {
    dup2(link[1], STDOUT_FILENO);
    close(link[0]); // read
    close(link[1]); // write
    char *path = getFilePath();
    if (execve(path, argv, getEnvironment()) == -1) {
        perror("execv() from run() in CgiBinRequest");
        exit(errno);
    }
    delete path;
    exit(0);
}

char *CgiBinRequest::getFilePath() {
    char *originalPath = (char *) filePath->c_str();
    char *end = index(originalPath, '?');
    if (end == nullptr) {
        end = originalPath + strlen(originalPath);
    }
    int delta = (int) (end - originalPath);
    char *path = new char[strlen(CGI_BIN_FOLDER) + delta + 1];
    strcpy(path, CGI_BIN_FOLDER);
    strncat(path, originalPath, (size_t) delta);
    return path;
}

void CgiBinRequest::listen() {
    close(link[1]); // write
    int bytes = 1;
    int total = 0;

    wait(nullptr); // should we wait first or consume/wait?
    while (bytes > 0) {
        memset(buffer, '\0', BUFFER_SIZE + 1);
        bytes = read(link[0], buffer, BUFFER_SIZE);

        if (bytes < 0) {
            perror("read()");
            exit(errno);
        } else {
            total += bytes;
            response->append(buffer, (unsigned long) bytes);
        }
    }
}

void CgiBinRequest::solve() {
    createPipe();

    debugEnvironmentVariables();

    pid_t pid = fork();

    if (pid == 0) {
        run(); // child
    } else {
        listen(); // parent, wait for child
    }
}

void CgiBinRequest::createPipe() {
    if (pipe(link) < 0) {
        perror("pipe");
        exit(errno);
    }
}

void CgiBinRequest::debugEnvironmentVariables() {
    auto env = getEnvironment();

    printf("Debugging environment variables...\n");
    while (*env) {
        printf("(%s)", *env);
        env++;
    }
    printf("Ended env-vars debugging\n");
}

char *CgiBinRequest::getResponse() {
    return (char *) this->response->c_str();
}


char **CgiBinRequest::getEnvironment() {
    int vars = getTotalEnvironmentCount();
    char **env = new char *[vars + 1]; // nullptr terminator
    char *temp = nullptr;

    int i = 0;

    while (environ[i] != nullptr) {
        temp = new char[strlen(environ[i]) + 1];
        strcpy(temp, environ[i]);
        env[i] = temp;
        i++;
    }
    for (int j = 0; j < this->extendedEnvironment->size(); ++j) {
        env[i] = (char *) this->extendedEnvironment->at(j).c_str();
        i++;
    }

    env[i] = nullptr;

    // Check for array overfloww
    if(i != vars) {
        printf("Expected set and allocated number of variables to be equal. %d != %d\n", i, vars);
        exit(99);
    }

    return env;
}

int CgiBinRequest::getTotalEnvironmentCount() const {
    int vars = getBaseEnvironmentCount();
    vars += extendedEnvironment->size();
    return vars;
}

int CgiBinRequest::getBaseEnvironmentCount() const {
    int vars = 0;
    while (environ[vars] != nullptr) {
        vars++;
    }
    return vars;
}

void CgiBinRequest::addEnvironmentVariables(string key, char *value) {
    addEnvironmentVariable((char*) key.c_str(), value);
}

void CgiBinRequest::addEnvironmentVariable(char *key, char *value) {
    char *env = new char[strlen(key) + strlen(value) + 2]; // '=' and \0

    strcpy(env, key);
    strcat(env, "=");
    strcat(env, value);

    this->extendedEnvironment->push_back(env);
}
