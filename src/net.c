#include "net.h"
#include "enc28j60.h"
#include "ethernet.h"
#include "ipv4.h"
#include "arp.h"
#include "string.h"
#include "netcommon.h"


// uint8_t gateway_mac[] = {0xB4, 0x2E, 0x99, 0xEC, 0x02, 0xC5};
uint8_t gateway_mac[] = {0xec, 0xc3, 0x02, 0xf1, 0x88, 0xb1};

void net_rx(uint8_t *buf) {
    struct enethdr *hdr = (struct enethdr *) buf;

    switch (hton16(hdr->type)) {
        case ETHERTYPE_IPV4:
            ipv4_deliver(&buf[ENET_DATA_OFFSET]);
            break;
        case ETHERTYPE_ARP:
            arp_deliver(&buf[ENET_DATA_OFFSET]);
        default:
            break;
    }
}

void net_tx(uint8_t *destmac, uint8_t *data, uint16_t len, uint16_t type) {
    uint8_t enet_frame[ENET_HEADER_SIZE + len];
    struct enethdr *hdr = (struct enethdr *) enet_frame;
    uint8_t src_mac[6];
    ENC28J60_get_mac_address(&ENC28J60, src_mac);
    memcpy(hdr->dest, destmac != 0 ? destmac : gateway_mac, 6);
    memcpy(hdr->src, src_mac, 6);
    memcpy(enet_frame + ENET_HEADER_SIZE, data, len);
    hdr->type = hton16(type);
    ENC28J60_write_frame_blocking(&ENC28J60, enet_frame, sizeof(enet_frame));
}

void net_mac_address(uint8_t *mac) {
    ENC28J60_get_mac_address(&ENC28J60, mac);
}
