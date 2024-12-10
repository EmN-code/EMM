
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/ipv6/addr.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netif.h"
#include "net/gnrc.h"

#define UDP_PORT 20001
#define UDP_REMOTE_ADDR "47.102.103.1"
#define UDP_REMOTE_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static char udp_payload[] = DEVICE_STATE;

static void send_udp_message(void)
{
    gnrc_pktsnip_t *payload, *udp, *ip;
    ipv6_addr_t remote_addr;
    uint16_t port = UDP_REMOTE_PORT;

    /* Convert remote address to IPv6 address */
    if (ipv6_addr_from_str(&remote_addr, UDP_REMOTE_ADDR) == NULL) {
        puts("Error: unable to parse remote address");
        return;
    }

    /* Allocate payload */
    payload = gnrc_pktbuf_add(NULL, udp_payload, strlen(udp_payload), GNRC_NETTYPE_UNDEF);
    if (payload == NULL) {
        puts("Error: unable to copy data to packet buffer");
        return;
    }

    /* Allocate UDP header, set source port := destination port */
    udp = gnrc_udp_hdr_build(payload, port, port);
    if (udp == NULL) {
        puts("Error: unable to allocate UDP header");
        gnrc_pktbuf_release(payload);
        return;
    }

    /* Allocate IPv6 header */
    ip = gnrc_ipv6_hdr_build(udp, NULL, &remote_addr);
    if (ip == NULL) {
        puts("Error: unable to allocate IPv6 header");
        gnrc_pktbuf_release(udp);
        return;
    }

    /* Send packet */
    if (!gnrc_netapi_dispatch_send(GNRC_NETTYPE_UDP, GNRC_NETREG_DEMUX_CTX_ALL, ip)) {
        puts("Error: unable to send UDP packet");
        gnrc_pktbuf_release(ip);
        return;
    }

    puts("UDP message sent");
}

int main(void)
{
    puts("RIOT UDP sender application");

    /* Initialize network stack */
    gnrc_netreg_entry_t entry = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL, thread_getpid());
    gnrc_netreg_register(GNRC_NETTYPE_UDP, &entry);

    while (1) {
        send_udp_message();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
