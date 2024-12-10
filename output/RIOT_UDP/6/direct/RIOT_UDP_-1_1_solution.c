#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "net/gnrc/netif.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/udp.h"
#include "net/ipv6/addr.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT "20001"
#define SEND_INTERVAL 5

static gnrc_netreg_entry_t server;

void *sender(void *arg) {
    (void)arg; // suppress "unused variable" warning
    char message[] = "work";
    char notwork_message[] = "not work";
    msg_t msg, reply;
    gnrc_pktsnip_t *payload, *udp, *ip;
    ipv6_addr_t addr;

    // Convert the server address
    ipv6_addr_from_str(&addr, SERVER_ADDR);

    // Register a gnrc application with high priority
    msg.type = GNRC_NETREG_DEMUX_CTX;
    msg.content.ptr = (char *)&server;
    int res = gnrc_netreg_register(GNRC_NETTYPE_UDP, &server);

    if (res < 0) {
        printf("Error registering UDP connection\n");
        return NULL;
    }

    while (1) {
        // Prepare the message with "work"
        payload = gnrc_pktbuf_add(NULL, message, sizeof(message), GNRC_NETTYPE_UNDEF);
        // Build UDP packet
        udp = gnrc_udp_hdr_build(payload, SERVER_PORT, SERVER_PORT);
        // Build IP packet
        ip = gnrc_ipv6_hdr_build(udp, NULL, (ipv6_addr_t *)&addr);

        // Send the packet
        if (gnrc_netif_send(NULL, ip) < 0) {
            puts("Error sending message\n");
        }

        // Wait for the interval
        sleep(SEND_INTERVAL);
    }

    return NULL;
}

int main(void) {
    printf("UDP state sender is starting\n");

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  sender, NULL, "sender");

    return 0;
}