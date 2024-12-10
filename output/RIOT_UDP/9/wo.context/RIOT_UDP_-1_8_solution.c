#include "xtimer.h"
#include "thread.h"
#include "net/protnum.h"
#include "net/gnrc/udp.h"
#include "net/ipv6/addr.h"
#include "log_module.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

static gnrc_netif_t *netif = NULL;

static void _initialize(void) {
    gnrc_netif_t *netif_ptr;
    netif_ptr = gnrc_netif_iter(NULL);
    if (netif_ptr != NULL) {
        netif = netif_ptr;
    }
}

void send_udp_packet(void) {
    gnrc_pktsnip_t *payload, *udp, *ip;
    gnrc_pktsnip_t *pkt;
    sock_udp_ep_t remote;
    uint8_t buf[64];
    int res;

    remote.family = AF_INET;
    remote.netif = sock_udp_ep_to_last_netif();
    remote.port = SERVER_PORT;

    // Convert IP address to binary form
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) <= 0) {
        LOG_ERROR("inet_pton() failed for address %s", SERVER_ADDR);
        return;
    }

    const char *state_msg = "work"; // or "not work" based on the device state

    // Initialize a message payload
    payload = gnrc_pktbuf_add(NULL, state_msg, strlen(state_msg), GNRC_NETTYPE_UNDEF);

    // add a UDP header
    udp = gnrc_udp_hdr_build(payload, NULL, 0, remote.port);

    // add an IP header
    ip = gnrc_ipv6_hdr_build(udp, NULL, 0);

    // create a packet from the output of the transformations
    pkt = pktbuf_create(ip);

    // send the UDP packet
    res = gnrc_netapi_send(netif->pid, pkt);

    if (res < 0) {
        LOG_ERROR("gnrc_netapi_send() failed");
    }
    else {
        LOG_INFO("Sent UDP packet with message: %s", state_msg);
    }
}

int main(void) {
    _initialize();

    while (1) {
        send_udp_packet();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}