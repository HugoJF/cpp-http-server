//
// Created by Hugo on 8/31/2018.
//

#include "HeaderBuilder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_COUNT 2
#define REALLOC_MULTIPLIER 2

HeaderBuilder::HeaderBuilder() {
    ResizeArray();
}

void HeaderBuilder::ResizeArray() {
    if (allocatedHeaders == 0) {
        allocatedHeaders = INITIAL_COUNT;
        headers = (Header **) malloc(sizeof(Header *) * allocatedHeaders);
        for (int i = 0; i < INITIAL_COUNT; ++i) {
            headers[i] = (Header *) malloc(sizeof(Header));
        }
    } else if (headerCount == allocatedHeaders) {
        allocatedHeaders *= REALLOC_MULTIPLIER;
        headers = (Header **) realloc(headers, sizeof(Header *) * allocatedHeaders);
        for (int i = allocatedHeaders / REALLOC_MULTIPLIER; i < allocatedHeaders; ++i) {
             headers[i] = (Header *) malloc(sizeof(Header));
        }
    }
}

void HeaderBuilder::AddHeader(char *key, char *value) {
    ResizeArray();
    headers[headerCount]->key = key;
    headers[headerCount++]->value = value;
}

Header *HeaderBuilder::GetHeader(char *key) {
    for (int i = 0; i < headerCount; ++i) {
        if(strcmp(key, GetHeader(i)->key) == 0) {
            return GetHeader(i);
        }
    }

    return nullptr;
}

Header *HeaderBuilder::GetHeader(int index) {
    return headers[index];
}

int HeaderBuilder::ComputeHeaderSize() {
    int size = 0;

    for (int i = 0; i < headerCount; ++i) {
        size += strlen(GetHeader(i)->key);
        size += strlen(GetHeader(i)->value);
        size += 4; // CRLF and ": "
    }

    printf("Compute size to be %d byte\n", size);

    return size;
}

char *HeaderBuilder::Render() {
    int computedSize = ComputeHeaderSize();
    char *render = (char *) malloc(sizeof(char) * (computedSize + 1));
    memset(render, 'a', computedSize + 1);
    char *current = render;
    size_t aux;

    printf("Rendering %d headers\n", headerCount);

    for (int i = 0; i < headerCount; ++i) {
        Header *h = GetHeader(i);
        printf("Rendering %s: %s\n", h->key, h->value);

        sprintf(current, "%s: %s\r\n", h->key, h->value);
        
        current += strlen(h->key) + strlen(h->value) + 4;
    }
    
    return render;
}
