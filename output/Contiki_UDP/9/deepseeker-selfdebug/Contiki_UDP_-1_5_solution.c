
#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "xtimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/netif.h"

#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_LOCAL_PORT 12345
#define DEVICE_STATE "work"  // or "not work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static gnrc_netreg_entry_t entry;

void udp_send_state(void) {
    gnrc_pktsnip_t *payload, *udp, *ip;
    uint8_t addr[16];
    uip_ipaddr_t server_addr;

    // Convert the server IP address from string to uip_ipaddr_t
    uip_ipaddr_from_str(&server_addr, UDP_SERVER_ADDR);

    // Create the payload
    payload = gnrc_pktbuf_add(NULL, DEVICE_STATE, strlen(DEVICE_STATE), GNRC_NETTYPE_UNDEF);
    if (payload == NULL) {
        puts("Error: unable to create payload.");
        return;
    }

    // Create the UDP header
    udp = gnrc_udp_hdr_build(payload, UDP_LOCAL_PORT, &server_addr, UDP_SERVER_PORT);
    if (udp == NULL) {
        puts("Error: unable to create UDP header.");
        gnrc_pktbuf_release(payload);
        return;
    }

    // Create the IP header
    ip = gnrc_ipv6_hdr_build(udp, NULL, &server_addr);
    if (ip == NULL) {
        puts("Error: unable to create IP header.");
        gnrc_pktbuf_release(udp);
        return;
    }

    // Send the packet
    if (!gnrc_netapi_dispatch_send(GNRC_NETTYPE_UDP, GNRC_NETREG_DEMUX_CTX_ALL, ip)) {
        puts("Error: unable to send UDP packet.");
        gnrc_pktbuf_release(ip);
    }
}

void *udp_thread(void *arg) {
    (void)arg;

    // Register the UDP port
    gnrc_netreg_entry_init_pid(&entry, UDP_LOCAL_PORT, sched_active_pid);
    gnrc_netreg_register(GNRC_NETTYPE_UDP, &entry);

    while (1) {
        udp_send_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("UDP Client Example");

    // Start the UDP thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, udp_thread, NULL, "udp_thread");

    return 0;
}
