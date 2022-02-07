#include <stdint.h>
#include "webserver.h"

static uint8_t WEBSERVER_RUNNING;

static void webserver_run(void);

void webserver_init() {
    return;
}

void webserver_start() {
    WEBSERVER_RUNNING = 1;
    webserver_run();
}

static void webserver_run(void) {
    while (WEBSERVER_RUNNING) {
        // accept client
        // read http request
        // serve request
        // close connection
    }
}