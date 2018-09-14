#include <iostream> // exit, printf, errno
#include <string.h> // memset
#include <fcntl.h> // open
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <src/HTTPRequest.h>
#include <src/Listener.h>
#include <src/Worker.h>
#include <src/HeaderBuilder.h>
#include <src/FileRequest.h>
#include <src/RequestSolver.h>
#include <src/CgiBinRequest.h>

#define PORT 30000

void RemoveStdoutBuffering();

void RegisterSignalHandler();

using namespace std;

void Boot();

void BuildSocket();

void Bind();

void Listen();

int Accept();

void *dispatchThread(void *args);


int main(int argc, char *args[], char *arge[]) {

    // Check Thread-Safety for multiple functions
    // Remove buffers (use strings)
    // Fix method naming
    // Remove random todo lists
    // Multi-threading tests
    // Commenting
    // Check 'const' keyword use
    // Remove Environment pointer tracking
    // Add comment why library is used
    // Add missing env-vars on CGI-BIN
    // Add timeout for empty-read to avoid locking
    // BUG: Acessing index.html then trying to access CGI-BIN glitches CGI-BIN Environment Variables, possible fix: environ pointer

    RegisterSignalHandler();
    RemoveStdoutBuffering();

    Boot();

    BuildSocket();

    Bind();

    Listen();

    int connections = 0;

    while (connections++ < 5000) {
        int newConnFd = Accept();
        auto *thread_id = new pthread_t();
        auto *attributes = new pthread_attr_t();

        printf("Dispatching new listener and worker thread....\n");
//        if(pthread_create(thread_id, nullptr, dispatchThread, (void*)&newConnFd)) {
//            perror("pthreads()");
//            exit(errno);
//        }
        dispatchThread((void *) &newConnFd);
    }
}

void *dispatchThread(void *args) {
    int *newConnFd = (int *) (args);

    printf("Received connection %d\n", newConnFd);

    auto *listener = new Listener(*newConnFd);

    auto *request = listener->readRequest();

    if (request == nullptr) {
        printf("Received empty request, skipping worker...\n");
    } else {
        auto *worker = new Worker(listener, request);

        worker->work();

        delete worker;
    }

    listener->close();
}

struct sockaddr_in *serverAddress;
uint16_t port = 8080;
int listenFd = 0;

void Boot() {
    serverAddress = new sockaddr_in();
    memset(serverAddress, '\0', sizeof(sockaddr_in));

    serverAddress->sin_family = AF_INET;
    serverAddress->sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress->sin_port = htons(port);
}

void BuildSocket() {
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
}

void Bind() {
    if (bind(listenFd, (struct sockaddr *) serverAddress, sizeof(sockaddr_in)) == -1) {
        perror("Bind()");
        exit(EXIT_FAILURE);
    }
}

void Listen() {
    if (listen(listenFd, 3)) {
        perror("Listen()");
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d\n", port);
}

int Accept() {
    printf("##### Waiting for new connection #####\n");
    int connectionFd = accept(listenFd, nullptr, nullptr);
    printf("Accepted new connection..\n");
    if (connectionFd == -1) {
        perror("Accept()");
        exit(EXIT_FAILURE);
    }
    return connectionFd;
}

void HandleSignal(int signum) {
    printf("Receiving signal %d\n");
}

void RegisterSignalHandler() {
    signal(SIGINT, HandleSignal);
}


void RemoveStdoutBuffering() { setbuf(stdout, nullptr); }