#include <stdint.h>
#include "webserver.h"
#include "http1_0.h"
#include "endpoint.h"

#define REQUEST_BUF_LEN 2000

static uint8_t WEBSERVER_RUNNING;

const struct endpoint ENDPOINTS[] = {
    {.path = "/", .GET = 0, .HEAD = 0, .POST = 0},
    {.path = "/about", .GET = 0, .HEAD = 0, .POST = 0},
};

struct socket *HTTP_SOCKET;

static void webserver_run(void);
static webserver_serve(struct http_request_message *req);

void webserver_init() {
    HTTP_SOCKET = socket_init(SOCKTYPE_TCP);
    socket_bind(HTTP_SOCKET, 80);
    return;
}

void webserver_start() {
    WEBSERVER_RUNNING = 1;
    webserver_run();
}

static void webserver_run(void) {
    uint8_t buf[REQUEST_BUF_LEN];
    struct http_request_message req;
    while (WEBSERVER_RUNNING) {
        socket_accept(HTTP_SOCKET);
        socket_read(HTTP_SOCKET, buf, 1000);
        http_parse_request(buf, &req);
        webserver_serve(&req);
        //socket_close(HTTP_SOCKET);
    }
}

static webserver_serve(struct http_request_message *req) {
    struct endpoint *ep = get_endpoint(req->uri, ENDPOINTS, sizeof(ENDPOINTS));
    void (*method)();

    if (ep == 0) {
        // return 404
    }

    switch (req->method) {
        GET:
            method = ep->GET;
            break;
        HEAD:
            method = ep->HEAD;
            break;
        POST:
            method = ep->POST;
            break;
        default:
            // return unknown method error
            break;
    }

    if (method != 0) {
        method(req);
    } else {
        // return method not allowed
    }
}