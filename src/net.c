#include "net.h"
#include "enc28j60.h"
#include "ethernet.h"
#include "ipv4.h"
#include "string.h"
#include "netcommon.h"

uint8_t gateway_mac[] = {0xD8, 0x47, 0x32, 0xFC, 0xFB, 0xF8};

void net_rx(uint8_t *buf) {
    struct enethdr *hdr = (struct enethdr *) buf;

    switch (hton16(hdr->type)) {
        case ETHERTYPE_IPV4:
            ipv4_deliver(&buf[ENET_DATA_OFFSET]);
            break;
        default:
            break;
    }
}

void net_tx(uint8_t *data, uint16_t len, uint16_t type) {
    uint8_t enet_frame[ENET_HEADER_SIZE + len];
    struct enethdr *hdr = (struct enethdr *) enet_frame;
    uint8_t src_mac[6];
    ENC28J60_get_mac_address(&ENC28J60, src_mac);
    memcpy(hdr->dest, gateway_mac, 6);
    memcpy(hdr->src, src_mac, 6);
    memcpy(enet_frame + ENET_HEADER_SIZE, data, len);
    hdr->type = hton16(type);
    ENC28J60_write_frame_blocking(&ENC28J60, enet_frame, sizeof(enet_frame));
}
