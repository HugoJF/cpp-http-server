//
// Created by Hugo on 8/31/2018.
//

#include "inc/HeaderBuilder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_COUNT 2
#define REALLOC_MULTIPLIER 2

HeaderBuilder::HeaderBuilder() {
    resizeArray();
}

void HeaderBuilder::resizeArray() {
    if (allocatedHeaders == 0) {
        allocatedHeaders = INITIAL_COUNT;
        headers = new Header *[allocatedHeaders];
        for (int i = 0; i < INITIAL_COUNT; ++i) {
            headers[i] = new Header();
        }
    } else if (headerCount == allocatedHeaders) {
        allocatedHeaders *= REALLOC_MULTIPLIER;
        headers = (Header **) realloc(headers, sizeof(Header *) * allocatedHeaders);
        for (int i = allocatedHeaders / REALLOC_MULTIPLIER; i < allocatedHeaders; ++i) {
            headers[i] = new Header();
        }
    }
}

void HeaderBuilder::addHeader(char *key, char *value) {
    resizeArray();
    headers[headerCount]->key = key;
    headers[headerCount++]->value = value;
}

Header *HeaderBuilder::getHeader(char *key) {
    for (int i = 0; i < headerCount; ++i) {
        if (strcmp(key, getHeader(i)->key) == 0) {
            return getHeader(i);
        }
    }

    return nullptr;
}

Header *HeaderBuilder::getHeader(int index) {
    return headers[index];
}

int HeaderBuilder::computeHeaderSize() {
    int size = 0;

    for (int i = 0; i < headerCount; ++i) {
        size += strlen(getHeader(i)->key);
        size += strlen(getHeader(i)->value);
        size += 4; // CRLF and ": "
    }

    printf("Compute size to be %d byte\n", size);

    return size;
}

char *HeaderBuilder::render() {
    int computedSize = computeHeaderSize();
    char *render = new char[computedSize + 1];
    memset(render, 'a', computedSize + 1);
    char *current = render;
    size_t aux;

    printf("Rendering %d headers\n", headerCount);

    for (int i = 0; i < headerCount; ++i) {
        Header *h = getHeader(i);
        printf("Rendering %s: %s\n", h->key, h->value);

        sprintf(current, "%s: %s\r\n", h->key, h->value);

        current += strlen(h->key) + strlen(h->value) + 4;
    }

    return render;
}
