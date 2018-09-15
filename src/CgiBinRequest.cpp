

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
#include "CgiBinRequest.h"
#include <src/HTTPRequest.h>

#define BUFFER_SIZE 4096
#define CGI_BIN_FOLDER ".."
// TODO: Avoid using fixed buffer, if response is bigger than BUFFER_SIZE overflow will occur

CgiBinRequest::CgiBinRequest(char *filePath, HTTPRequest *httpRequest) {
    this->httpRequest = httpRequest;
    this->buffer = new char[BUFFER_SIZE];
    this->extendedEnvironment = new vector<string>();
    this->argv = new char *[1];
    this->filePath = new string(filePath);
    this->response = new string;
    argv[0] = nullptr;
    printf("CGI-BIN Path: %s\n", this->getFilePath());
}

void CgiBinRequest::run() {
    dup2(link[1], STDOUT_FILENO);
    close(link[0]); // read
    close(link[1]); // write
    if (execve(getFilePath(), argv, getEnvironment()) == -1) {
        perror("execv() from run() in CgiBinRequest");
        exit(errno);
    }
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
    _ssize_t bytes = 1;
    _ssize_t total = 0;

    wait(nullptr); // should we wait first or consume/wait?
    while (bytes > 0) {
        memset(buffer, '\0', BUFFER_SIZE);
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

    if (pid == 0) { // child
        run();
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
    while(*env) {
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

    vars = 0;

    while (environ[vars] != nullptr) {
        env[vars] = environ[vars];
        vars++;
    }
    for (int i = 0; i < this->extendedEnvironment->size(); ++i) {
        env[vars] = (char *) this->extendedEnvironment->at(i).c_str();
        vars++;
    }

    env[vars] = nullptr;
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

void CgiBinRequest::addEnvironmentVariableS(string key, char *value) {
    char *env = new char[strlen(key.c_str()) + strlen(value) + 2];

    // snprintf pls
    strcpy(env, key.c_str());
    strcat(env, "=");
    strcat(env, value);

    this->extendedEnvironment->push_back(env);
}
void CgiBinRequest::addEnvironmentVariable(char *key, char *value) {
    char *env = new char[strlen(key) + strlen(value) + 2];

    // snprintf pls
    strcpy(env, key);
    strcat(env, "=");
    strcat(env, value);

    this->extendedEnvironment->push_back(env);
}
