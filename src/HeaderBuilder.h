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

    void ResizeArray();
    int ComputeHeaderSize();

public:
    HeaderBuilder();

    void AddHeader(char *key, char *value);
    Header* GetHeader(char *key);
    Header* GetHeader(int index);

    char *Render();
};


#endif //SIMPLE_HTTP_SERVER_HEADERBUILDER_H
