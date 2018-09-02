#include <iostream> // exit, printf, errno
#include <string.h> // memset
#include <fcntl.h> // open
#include <unistd.h>
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

int main(int argc, char *args[], char *arge[]) {

    // Move socket to server
    // Move binder to socket
    // Check Thread-Safety for multiple functions

    RegisterSignalHandler();
    RemoveStdoutBuffering();


    CgiBinRequest *cg = new CgiBinRequest("../cgi-bin/sum.py", nullptr);

    cg->setEnvironmentVariables(arge);

    cg->solve();

    printf("-> %s", cg->getResponse()->c_str());

    exit(0);

    auto *listener = new Listener();

    listener->Boot();

    listener->BuildSocket();

    listener->Bind();

    listener->Listen();

    int connections = 0;

    while (connections++ < 50) {
        listener->Accept();

        auto *request = listener->ReadRequest();

        auto *worker = new Worker(listener, request);

        worker->Work();

        listener->Close();
    }
}

void HandleSignal(int signum) {
    printf("Receiving signal %d\n");
}

void RegisterSignalHandler() {
    signal(SIGINT, HandleSignal);
}


void RemoveStdoutBuffering() { setbuf(stdout, 0); }