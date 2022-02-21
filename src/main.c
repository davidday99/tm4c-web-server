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
#include "webserver.h"

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

    // ipv4_set_address(0xC0A8006F);  // set IP address as 192.168.0.111
    ipv4_set_address(ipv4_to_int("192.168.1.150"));

    EnableInterrupts();

    webserver_init();
    webserver_start();
}