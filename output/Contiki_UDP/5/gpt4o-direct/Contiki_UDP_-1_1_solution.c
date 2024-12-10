#include "net/ipv4/ipv4.h"
#include "net/gnrc/netif.h"
#include "net/gnrc.h"
#include "ztimer.h"
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds
#define STATE_WORKING "work"
#define STATE_NOT_WORKING "not work"

static gnrc_netif_t *netif = NULL;

void send_udp_message(const char *state) {
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);
    if (!netif) {
        puts("No network interface found.");
        return;
    }

    ipv4_addr_t addr;
    if (ipv4_addr_from_str(&addr, SERVER_ADDR) == NULL) {
        puts("Failed to parse server address.");
        return;
    }

    gnrc_pktsnip_t *payload, *udp, *ip;

    payload = gnrc_pktbuf_add(NULL, state, strlen(state), GNRC_NETTYPE_UNDEF);
    udp = gnrc_udp_hdr_build(payload, NULL, 0, SERVER_PORT);
    if (udp == NULL) {
        gnrc_pktbuf_release(payload);
        puts("Could not allocate memory for UDP header.");
        return;
    }

    ip = gnrc_ipv4_hdr_build(udp, NULL, addr.uint32, 255);
    if (ip == NULL) {
        gnrc_pktbuf_release(udp);
        puts("Could not allocate memory for IPv4 header.");
        return;
    }

    if (gnrc_netapi_send(netif->pid, ip) < 1) {
        gnrc_pktbuf_release(ip);
        puts("Error sending packet.");
    } else {
        printf("Sending "%s" to server %s:%d\n", state, SERVER_ADDR, SERVER_PORT);
    }
}

int main(void) {
    puts("Starting UDP client example...");

    netif = gnrc_netif_iter(NULL);
    if (!netif) {
        puts("Error: No network interface found");
        return -1;
    }

    ztimer_timer_t timer;
    ztimer_init(ZTIMER_USEC, &timer, send_udp_message, STATE_WORKING);

    while (1) {
        ztimer_tick_t last_wakeup = ztimer_now(ZTIMER_USEC);
        send_udp_message(STATE_WORKING);
        ztimer_sleep_until(ZTIMER_USEC, last_wakeup + INTERVAL);
    }

    return 0;
}
