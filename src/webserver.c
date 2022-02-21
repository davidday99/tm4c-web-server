#include <stdint.h>
#include "webserver.h"
#include "endpoint.h"
#include "http1_0.h"
#include "socket.h"
#include "lcd.h"
#include "string.h"

#define REQUEST_BUF_LEN 2000

static uint8_t WEBSERVER_RUNNING;

extern void index(struct socket *conn, struct http_request_message *req);
extern void about(struct socket *conn, struct http_request_message *req);

const struct endpoint ENDPOINTS[] = {
    {.path = "/", .GET = index, .HEAD = 0, .POST = 0},
    {.path = "/about", .GET = about, .HEAD = 0, .POST = 0},
};

static void webserver_run(void);
static void webserver_serve(struct socket *conn, struct http_request_message *req);
void log_client(struct socket *conn);

static struct socket *client;

extern LCD lcd;

void webserver_init() {
    client = socket_init(SOCKTYPE_TCP);
    socket_bind(client, 80);
    return;
}

void webserver_start() {
    WEBSERVER_RUNNING = 1;
    webserver_run();
}

void webserver_stop() {
    WEBSERVER_RUNNING = 0;
}

static void webserver_run(void) {
    uint8_t data[REQUEST_BUF_LEN];
    uint16_t len = 0;
    struct http_request_message req;

    while (WEBSERVER_RUNNING) {
        // accept client
        if (socket_accept(client) == 0)
            continue;

        // read http request
        len = socket_read(client, data, REQUEST_BUF_LEN);

        // serve request
        if (len > 0) {
            log_client(client);
            http_parse_request(data, &req);
            webserver_serve(client, &req);
        }

        // close connection
        socket_close(client);
    }
}

static void webserver_serve(struct socket *conn, struct http_request_message *req) {
    const struct endpoint *ep = get_endpoint(req->uri, ENDPOINTS, sizeof(ENDPOINTS));
    void (*method)() = 0;

    if (ep == 0) {
        // return 404
        while (1)
            ;
    }

    switch (req->method) {
        case GET:
            method = ep->GET;
            break;
        case HEAD:
            method = ep->HEAD;
            break;
        case POST:
            method = ep->POST;
            break;
        default:
            // return unknown method error
            break;
    }

    if (method != 0) {
        method(conn, req);
    } else {
        // return method not allowed
    }
}

void log_client(struct socket *conn) {
    char ip[16];
    int_to_ipv4(conn->clientaddr.ip, ip);
    lcd_write(&lcd, "Client ");
    lcd_write(&lcd, ip);
    lcd_write(&lcd, "\n");
}