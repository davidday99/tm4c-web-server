#include <stdint.h>
#include "endpoint.h"
#include "string.h"

const struct endpoint *get_endpoint(char *path, const struct endpoint epts[], uint8_t eptcount) {
    for (uint8_t i = 0; i < eptcount; i++)
        if (strcmp(path, epts[i].path) == 0)
            return &epts[i];
    return 0;
}