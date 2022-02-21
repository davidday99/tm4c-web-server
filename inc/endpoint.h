#ifndef _ENDPOINT_H_
#define _ENDPOINT_H_

#include <stdint.h>

struct endpoint {
    char *path;
    void (*GET)();
    void (*HEAD)();
    void (*POST)();
};

const struct endpoint *get_endpoint(char *path, const struct endpoint epts[], uint8_t eptcount);

#endif /* _ENDPOINT_H_ */