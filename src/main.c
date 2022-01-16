#include "system.h"
#include "ssi.h"
#include "gpio.h"
#include "tm4c123gh6pm.h"
#include "st7735.h"
#include "enc28j60.h"
#include "lcd.h"
#include "socket.h"

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

static uint8_t frame[] = {
    0xB4,
    0x2E,
    0x99,
    0xEC,
    0x02,
    0xC5,
    0xDE,
    0xAD,
    0xBE,
    0xEF,
    0xCC,
    0xCC,
    0x08,
    0x00,
    0x45,
    0x00, 
    0x00,
    0x21,
    0xFF,
    0xFF,
    0x40,
    0x00,
    0x40,
    0x11,
    0x00,
    0x00,
    0xC0,
    0xA8,
    0x00,
    0x01,
    0xc0,
    0xa8,
    0x00,
    0x6E,
    0x00,
    0x50,
    0x00,
    0x50,
    0x00,
    0x0D,
    0x00,
    0x00,
    'O',
    'K',
    'A',
    'Y',
    0
};

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

    struct socket *sock = socket_init(SOCKTYPE_UDP);
    struct socket_addr sockaddr = {SOCKADDR_IP_ANY, 5353};
    socket_bind(sock, &sockaddr);
    uint8_t data[64];
    uint8_t reply[] = "Hello, world!\n";

    EnableInterrupts();

    // ENC28J60_write_frame_blocking(&ENC28J60, frame, sizeof(frame));

    int i = 0;
    int len;
    while (1) {
        len = socket_read(sock, &sockaddr, data, 64);
        if (data[0] != '\0') {
            data[len] = '\0';
            lcd_write(&lcd, "%d SOCK:", i++);
            lcd_write(&lcd, (char *) data);
            lcd_write(&lcd, "\n");
            data[0] = '\0';
        }
        socket_sendto(sock, &sockaddr, reply, sizeof(reply));
        // Delay(10);
    }
}