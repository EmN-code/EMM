
#include <stdio.h>
#include <string.h>
#include "net/ipv6/addr.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/udp.h"
#include "net/gnrc.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char packet_buf[128];

void send_udp_message(void) {
    gnrc_pktsnip_t *payload, *udp, *ip;
    uint8_t ip_proto = GNRC_NETTYPE_IPV6;

    // Create UDP payload
    payload = gnrc_pktbuf_add(NULL, DEVICE_STATE_MSG, strlen(DEVICE_STATE_MSG), GNRC_NETTYPE_UNDEF);
    if (payload == NULL) {
        puts("Error: unable to create UDP payload");
        return;
    }

    // Create UDP header
    udp = gnrc_udp_hdr_build(payload, SERVER_PORT, SERVER_PORT);
    if (udp == NULL) {
        puts("Error: unable to create UDP header");
        gnrc_pktbuf_release(payload);
        return;
    }

    // Create IPv6 header
    ip = gnrc_ipv6_hdr_build(udp, NULL, SERVER_IP);
    if (ip == NULL) {
        puts("Error: unable to create IPv6 header");
        gnrc_pktbuf_release(udp);
        return;
    }

    // Send packet
    if (!gnrc_netapi_dispatch_send(GNRC_NETTYPE_UDP, GNRC_NETREG_DEMUX_CTX_ALL, ip)) {
        puts("Error: unable to send UDP packet");
        gnrc_pktbuf_release(ip);
        return;
    }

    puts("UDP message sent");
}

int main(void) {
    puts("UDP client starting");

    while (1) {
        send_udp_message();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
