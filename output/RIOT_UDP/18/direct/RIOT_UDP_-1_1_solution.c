#include <xtimer.h>
#include <net/gnrc/udp.h>
#include <net/af.h>
#include <net/gnrc/netif.h>
#include <arpa/inet.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

void send_udp_message(const char *message) {
    ipv6_addr_t addr;
    gnrc_pktsnip_t *payload, *udp, *ip;
    gnrc_udp_hdr_t *udphdr;
    uint16_t msg_size = strlen(message);

    // Allocate payload
    payload = gnrc_pktbuf_add(NULL, message, msg_size, GNRC_NETTYPE_UNDEF);
    if (payload == NULL) {
        printf("Error: Unable to allocate payload.\n");
        return;
    }

    // Allocate UDP header
    udp = gnrc_udp_hdr_build(payload, SERVER_PORT, SERVER_PORT);
    if (udp == NULL) {
        printf("Error: Unable to allocate UDP header.\n");
        gnrc_pktbuf_release(payload);
        return;
    }

    // Allocate IP header
    ip = gnrc_ipv6_hdr_build(udp, NULL, &addr);
    if (ip == NULL) {
        printf("Error: Unable to allocate IPV6 header.\n");
        gnrc_pktbuf_release(udp);
        return;
    }

    // Resolve server address
    if (ipv6_addr_from_str(&addr, SERVER_ADDR) == NULL) {
        printf("Error: Unable to parse IPV6 address '\n");
        gnrc_pktbuf_release(ip);
        return;
    }

    // Send message
    if (gnrc_netapi_send(gnrc_netif_iter(NULL), ip) < 1) {
        printf("Error: Unable to send message.\n");
        gnrc_pktbuf_release(ip);
        return;
    }

    printf("Message sent: %s\n", message);
}

int main(void) {
    while (1) {
        send_udp_message("work");
        xtimer_sleep(INTERVAL);
        send_udp_message("not work");
        xtimer_sleep(INTERVAL);
    }
    return 0;
}