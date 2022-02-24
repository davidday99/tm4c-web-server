#include <stdint.h>
#include "webserver.h"
#include "endpoint.h"
#include "http1_0.h"
#include "socket.h"
#include "lcd.h"
#include "string.h"
#include "timer_module.h"

#define REQUEST_BUF_LEN 2000
#define ENDPOINT_COUNT 2
#define CONNECTION_TIMEOUT 160000000

static uint8_t WEBSERVER_RUNNING;

void index(struct socket *conn, struct http_request_message *req);
void about(struct socket *conn, struct http_request_message *req);
static void webserver_run(void);
static void webserver_serve(struct socket *conn, struct http_request_message *req);
void log_client(struct socket *conn);

const struct endpoint ENDPOINTS[] = {
    {.path = "/", .GET = index, .HEAD = 0, .POST = 0},
    {.path = "/about", .GET = about, .HEAD = 0, .POST = 0},
};

struct socket *client;

extern LCD lcd;

extern struct TIMER *timeout;

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
        do {
            set_timer_32_bit_starting_value(timeout, CONNECTION_TIMEOUT);
            start_timer(timeout);
        } while (socket_accept(client) == 0);
        stop_timer(timeout);

        // read http request
        len = socket_read(client, data, REQUEST_BUF_LEN);

        // serve request
        if (len > 0) {
            http_parse_request(data, &req);
            webserver_serve(client, &req);
            log_client(client);
        }

        // close connection
        socket_close(client);
    }
}

static void webserver_serve(struct socket *conn, struct http_request_message *req) {
    lcd_write(&lcd, req->uri);
    lcd_write(&lcd, "\n");
    const struct endpoint *ep = get_endpoint(req->uri, ENDPOINTS, ENDPOINT_COUNT);
    void (*method)() = 0;

    if (ep == 0) {
        http_respond(conn, 404, "http://192.168.1.150/", "<h1>Error 404</h1><p> Page not found.</p>");
        return;
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
    static uint32_t hits;
    char ip[16];
    int_to_ipv4(conn->clientaddr.ip, ip);
    lcd_write(&lcd, "Req: ");
    lcd_write(&lcd, ip);
    lcd_write(&lcd, "\n");
    lcd_write(&lcd, "hit %d\n", ++hits);
}