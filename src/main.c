#include <stdint.h>
#include <string.h>

int main(void){
    uint8_t x[] = {1,2,3};
    uint8_t y[] = {0,0,0};

    /* always works */
    memcpy(y, x, 3);

    /* doesn't always work */
    uint8_t *xptr = x;
    uint8_t *yptr = y;
    memcpy(yptr, xptr, 3);

    while (1)
        ;
}