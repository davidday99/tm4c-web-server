#include <stdint.h>
#include "gpio.h"
#include "gpio_module.h"
#include "common.h"
#include "tm4c123gh6pm.h"
#include "dma_module.h"
#include "timer_module.h"
#include "enc28j60.h"
#include "ipv4.h"
#include "net.h"
#include "lcd.h"
#include "socket.h"

extern LCD lcd;
extern struct ENC28J60 *enc;
extern struct TIMER *timeout;
extern struct socket *client;
 

void GPIOPortB_ISR(void) {
    uint8_t pin = get_gpio_masked_interrupt_status(PORTB);
    mask_gpio_interrupt((&ENC28J60)->intr);

    /* Check which pin on PORTB triggered the interrupt. PIN2 is connected to the
        ethernet controller's INT pin.
    */
    if (pin & 2) {
        ENC28J60_disable_interrupts(&ENC28J60);
        clear_gpio_interrupt((&ENC28J60)->intr);
        uint8_t requests = ENC28J60_get_interrupt_requests(&ENC28J60);
        if (requests & 0x40) {
            set_timer_32_bit_starting_value((&ENC28J60)->timeout_clk, (&ENC28J60)->timeout_clk->start_low);
            ENC28J60_decrement_packet_count(&ENC28J60);
            ENC28J60_read_frame_dma(&ENC28J60);
        }
    }

    unmask_gpio_interrupt((&ENC28J60)->intr);
}

void SPI1_ISR(void) {
    if (dma_interrupt_occurred(enc->dmatx) || dma_interrupt_occurred(enc->dmarx)) {
        clear_dma_interrupt(enc->dmatx);
        clear_dma_interrupt(enc->dmarx);
    }

    if (remaining_dma_transfer_count(enc->dmarx) == 0) {
        set_gpio_pin_high(enc->cs);
        ENC28J60_disable_dma(enc);
        disable_ssi_interrupts(enc->ssi);
        net_rx(&(enc->rx_buf[1]));
        ENC28J60_advance_rdptr(enc);
        ENC28J60_enable_interrupts(enc);
    }
}

void Timer0A_ISR(void) {
    disable_timer_timeout_interrupt((&ENC28J60)->timeout_clk);
    clear_timer_timeout_interrupt((&ENC28J60)->timeout_clk);
    lcd_clear(&lcd);
    lcd_write(&lcd, "ENC TIMEOUT.\n");
    lcd_write(&lcd, "REBOOTING...\n");
    ENC28J60_init(&ENC28J60);
    ENC28J60_enable_receive(&ENC28J60);
    start_timer((&ENC28J60)->timeout_clk);
    enable_timer_timeout_interrupt((&ENC28J60)->timeout_clk);
}

void Timer1A_ISR(void) {
    disable_timer_timeout_interrupt(timeout);
    clear_timer_timeout_interrupt(timeout);
    client->tcb.state = CLOSED;
    enable_timer_timeout_interrupt(timeout);
}
