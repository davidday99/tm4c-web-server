#include "system.h"
#include "ssi.h"
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "st7735.h"
#include "enc28j60.h"
#include "lcd.h"
#include "socket.h"
#include "ipv4.h"
#include "string.h"
#include "http1_0.h"
#include "netcommon.h"

extern void EnableInterrupts();
extern void DisableInterrupts();
extern void StartCritical();
extern void EndCritical();

LCD lcd;
struct ENC28J60 *enc = &ENC28J60;

void Delay(uint32_t d) {
    uint32_t volatile delay = d * 80000;
    while (delay > 0)
        delay--;
}

int main(void){

    DisableInterrupts();

    PLL_init();

    lcd_init(&lcd);
    
    if (ENC28J60_init(enc)) {
        lcd_write(&lcd, "ENC initialized.\n");
        ENC28J60_enable_receive(enc) ? lcd_write(&lcd, "Receive enabled.\n") : 
                                    lcd_write(&lcd, "Could not enable.\n");
    } else {
        lcd_write(&lcd, "Could not init.\n");
    }

    ipv4_set_address(0xC0A8006F);  // set IP address as 192.168.0.111

    struct socket *sock = socket_init(SOCKTYPE_TCP);
    socket_bind(sock, 80);
    uint8_t data[2000];

    EnableInterrupts();

    int len;
    char buf[100];
    char ip[16];
    char *header = "<h1>Love u honey</h1><p>Your public IP is: ";
    uint8_t headerlen = strlen(header);

    socket_accept(sock);
    while (1) {
        len = socket_read(sock, data, 1000);
        if (len == 0) {
            socket_accept(sock);
            continue;
        }
        if (len > 0) {
            data[len] = '\0';
            // lcd_write(&lcd, (char *) data);
            int_to_ipv4(sock->clientaddr.ip, ip);
            lcd_write(&lcd, "Client ");
            lcd_write(&lcd, ip);
            lcd_write(&lcd, "\n");
            strcpy(buf, header);
            strcpy(buf + headerlen, ip);
            strcpy(buf + headerlen + strlen(ip), ".</p>");
            http_respond(sock, 200, "http://192.168.1.111/", buf);
            // socket_close(sock);
        }
    }
}