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
    struct socket_addr sockaddr = {SOCKADDR_IP_ANY, 8000};
    socket_bind(sock, &sockaddr);
    uint8_t data[64];

    EnableInterrupts();

    while (1) {
        socket_read(sock, &sockaddr, data, 64);
        if (data[0] != '\0') {
            lcd_write(&lcd, (char *) data);
            lcd_write(&lcd, "\n");
            data[0] = '\0';
        }
    }
}