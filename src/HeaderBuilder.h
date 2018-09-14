//
// Created by Hugo on 8/31/2018.
//

#ifndef SIMPLE_HTTP_SERVER_HEADERBUILDER_H
#define SIMPLE_HTTP_SERVER_HEADERBUILDER_H

typedef struct header_ {
    char *key;
    char *value;
} Header;

class HeaderBuilder {
private:
    Header **headers;
    int allocatedHeaders = 0;
    int headerCount = 0;

    void resizeArray();

    int computeHeaderSize();

public:
    HeaderBuilder();

    void addHeader(char *key, char *value);

    Header *getHeader(char *key);

    Header *getHeader(int index);

    char *render();
};


#endif //SIMPLE_HTTP_SERVER_HEADERBUILDER_H
