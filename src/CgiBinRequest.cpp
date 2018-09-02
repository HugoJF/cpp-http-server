

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

CgiBinRequest::CgiBinRequest(string filePath, HTTPRequest *httpRequest) {
    this->filePath = std::move(filePath);
    this->httpRequest = httpRequest;
    this->buffer = new char[BUFFER_SIZE];
    this->extendedEnvironment = new vector<string>();
    this->argv = new char *[1];
    response = new string();
    argv[0] = nullptr;
//    this->envp = new char*[2];
//    envp[0] = "PATH=POTATO";
//    envp[1] = nullptr;

    printf("CGI-BIN Path: %s\n", this->getFilePath());
    if (pipe(link) < 0) {
        perror("pipe");
        exit(errno);
    }
}

void CgiBinRequest::run() {
    dup2(link[1], STDOUT_FILENO);
    close(link[0]); // read
    close(link[1]); // write
    if (execve(getFilePath(), argv, getEnvironment()) == -1) {
        //if (execv(filePath, argv) == -1) {
        perror("execv() from run() in CgiBinRequest");
        exit(errno);
    }
    exit(0);
}

char *CgiBinRequest::getFilePath() {
    char *originalPath = (char*)filePath.c_str();
    char *end = index(originalPath, '?');
    int delta = (int) (end - originalPath);
    char *path = new char[strlen(CGI_BIN_FOLDER) + delta + 1];
    strcpy(path, CGI_BIN_FOLDER);
    strncat(path, originalPath, delta);
    return path;
}

void CgiBinRequest::listen() {
    close(link[1]); // write
    _ssize_t bytes = 1;
    _ssize_t total = 0;
    wait(nullptr);

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
    pid_t pid = fork();

    if (pid == 0) { // child
        run();
    } else {
        listen(); // parent, wait for child
    }
}

char *CgiBinRequest::getResponse() {
    return (char *) this->response->c_str();
}

// TODO: prettier way to get variables
// set reference for cloning
// set new variables
// then combine (avoid re-allocating big array)
// replace char*
// use vectors
void CgiBinRequest::setEnvironmentVariables(char **envp) {
    this->baseEnvironment = envp;
}

char **CgiBinRequest::getEnvironment() {
    int vars = 0;
    while (this->baseEnvironment[vars]) {
        vars++;
    }
    vars += this->extendedEnvironment->size();
    char **env = new char *[vars+1];
    vars = 0;
    while (this->baseEnvironment[vars]) {
        env[vars] = this->baseEnvironment[vars];
        vars++;
    }
    for (int i = 0; i < this->extendedEnvironment->size(); ++i, vars++) {
        env[vars] = (char *) this->extendedEnvironment->at(i).c_str();
    }

    env[vars] = nullptr;

    return env;
}

void CgiBinRequest::addEnvironmentVariable(char *key, char *value) {
    char *env = new char[strlen(key) + strlen(value) + 2];

    strcpy(env, key);
    strcat(env, "=");
    strcat(env, value);

    this->extendedEnvironment->push_back(env);
}
