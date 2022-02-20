#include <stdint.h>
#include "webserver.h"
#include "socket.h"
#include "http1_0.h"
#include "lcd.h"
#include "string.h"

static uint8_t WEBSERVER_RUNNING;

static void webserver_run(void);

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
    uint8_t data[1024];
    uint16_t len = 0;
    char buf[100];
    char ip[16];
    char *header = "<h1>Love u honey</h1><p>Your public IP is: ";
    uint8_t headerlen = strlen(header);

    while (WEBSERVER_RUNNING) {
        // accept client
        if (socket_accept(client) == 0)
            continue;

        // read http request
        len = socket_read(client, data, 1000);

        // serve request
        if (len > 0) {
            data[len] = '\0';
            // lcd_write(&lcd, (char *) data);
            int_to_ipv4(client->clientaddr.ip, ip);
            lcd_write(&lcd, "Client ");
            lcd_write(&lcd, ip);
            lcd_write(&lcd, "\n");
            strcpy(buf, header);
            strcpy(buf + headerlen, ip);
            strcpy(buf + headerlen + strlen(ip), ".</p>");
            http_respond(client, 200, "http://192.168.1.111/", buf);
        }

        // close connection
        socket_close(client);
    }
}