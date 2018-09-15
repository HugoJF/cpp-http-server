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
    /// Header array
    Header **headers;

    /// Currently allocated headers
    int allocatedHeaders = 0;

    /// Current header count
    int headerCount = 0;

    /// Resize array if it's full
    void resizeArray();

    /// Computes how many bytes are needed to "render" all headers
    /// \return - byte count for rendered headers
    int computeHeaderSize();

public:
    HeaderBuilder();

    /// Adds new header to array
    /// \param key - header key
    /// \param value - header value
    void addHeader(char *key, char *value);

    /// Get header with given key
    /// \param key - header key
    /// \return - Header struct
    Header *getHeader(char *key);

    /// Get header with given index
    /// \param index - header index
    /// \return - Header struct
    Header *getHeader(int index);

    /// \return - Renders headers into a string
    char *render();
};


#endif //SIMPLE_HTTP_SERVER_HEADERBUILDER_H
