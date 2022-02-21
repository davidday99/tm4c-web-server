#include <stdint.h>
#include "endpoint.h"
#include "http1_0.h"
#include "socket.h"

static char *about_html = "<h1>About</h1>" \
"<p>This is a webserver running on a TM4C microcontroller.</p>";


static char *links = "<p><a href=/about>About</a></p>" \
"<p><a href=https://github.com/davidday99/tm4c-web-server>Github</a></p>";

void index(struct socket *conn, struct http_request_message *req) {
    char buf[200];
    char ip[16];
    char *header = "<h1>Home</h1><p>Your public IP is: ";
    uint8_t headerlen = strlen(header);
    int_to_ipv4(conn->clientaddr.ip, ip);
    strcpy(buf, header);
    strcpy(buf + headerlen, ip);
    strcpy(buf + headerlen + strlen(ip), ".</p>");
    strcpy(buf + headerlen + strlen(ip) + strlen(".</p>"), links); 
    http_respond(conn, 200, "http://192.168.1.150/", buf);
}

void about(struct socket *conn, struct http_request_message *req) {
    http_respond(conn, 200, "http://192.168.1.150/", about_html);
}