#include <string.h> // memset
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "pthread.h"
#include <mutex>
#include "inc/HTTPRequest.h"
#include "inc/Listener.h"
#include "inc/Worker.h"
#include "inc/HeaderBuilder.h"
#include "inc/FileRequest.h"
#include "inc/RequestSolver.h"
#include "inc/CgiBinRequest.h"

#define PORT 8889
#define MAX_CONNECTIONS 5000000
#define USE_THREADS 1
#define REDIRECT_STDERR 1

void removeStdoutBuffering();

void registerSignalHandler();

using namespace std;

void boot();

void buildSocket();

void bind();

void listen();

int accept();

void *dispatchThread(void *args);

struct sockaddr_in *serverAddress;
int listenFd = 0;
int connections = 0;
std::mutex mtx;

int main(int argc, char *args[], char *arge[]) {

    // HTTPRequest should use HeaderBuild to store headers
    // Add comment why library is used
    // Add missing env-vars on CGI-BIN specified on RFC
    // Add timeout for empty-read to avoid locking
    // Keep-Alive connection support
    // Use vector in HeaderBuilder

    registerSignalHandler();
    removeStdoutBuffering();

    boot();

    buildSocket();

    bind();

    listen();

    while (connections++ < MAX_CONNECTIONS) {
        int connectionDescriptor = accept();

        printf("Dispatching thread... ");
#if USE_THREADS
        auto *thread = new pthread_t();
        auto *attributes = new pthread_attr_t();

        if (pthread_create(thread, nullptr, dispatchThread, (void *) &connectionDescriptor)) {
            perror("pthreads()");
            exit(errno);
        }
#else
        dispatchThread((void *) &connectionDescriptor);
#endif
    }

    close(listenFd);
}

void *dispatchThread(void *args) {
    int connectionDescriptor = (*(int *) (args));

    printf("Dispatched on descriptor: %d.\n", connectionDescriptor);

    mtx.lock();

    auto *listener = new Listener(connectionDescriptor);

    auto *request = listener->readRequest();

    mtx.unlock();

    if (request == nullptr) {
        printf("Received nullptr request, skipping worker dispatch...\n");
    } else {
        auto *worker = new Worker(listener, request);

        worker->work();

        delete worker;
    }

    listener->close();

    close(connectionDescriptor);

    delete listener;

    printf("REQUEST THREAD ENDED\n");
}

void boot() {
    serverAddress = new sockaddr_in();
    memset(serverAddress, '\0', sizeof(sockaddr_in));

    serverAddress->sin_family = AF_INET;
    serverAddress->sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress->sin_port = htons(PORT);
}

void buildSocket() {
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
}

void bind() {
    if (bind(listenFd, (struct sockaddr *) serverAddress, sizeof(sockaddr_in)) == -1) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }
}

void listen() {
    if (listen(listenFd, 3)) {
        perror("listen()");
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d\n", PORT);
}

int accept() {
    printf("Waiting new connections... ");
    int connectionFd = ::accept(listenFd, nullptr, nullptr);
    printf("ACCEPTED!\n");

    if (connectionFd == -1) {
        perror("Accept()");
        exit(EXIT_FAILURE);
    }

    return connectionFd;
}

void handleSignal(int signum) {
    printf("->Receiving signal %d\n", signum);
}

void registerSignalHandler() {
    signal(SIGINT, handleSignal);
}


void removeStdoutBuffering() {
    setbuf(stdout, nullptr);
    setbuf(stderr, nullptr);

    // Redirects stderr to stdout to avoid CLion bug: https://youtrack.jetbrains.com/issue/IDEA-70016
#if REDIRECT_STDERR
    dup2(STDOUT_FILENO, STDERR_FILENO);
#endif
}