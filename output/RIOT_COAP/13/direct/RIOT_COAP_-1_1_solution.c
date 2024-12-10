#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gnrc/netif.h"
#include "msg.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT "5683"
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static gnrc_netif_t *netif = NULL;  // Network interface

void find_network_interface(void) {
    netif = gnrc_netif_iter(NULL);
    if (!netif) {
        puts("Unable to find a network interface");
    }
}

void init_sockets_and_send_coap(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = atoi(SERVER_PORT) };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_ADDR);

    if (!netif) {
        puts("Network interface not found. Check your configuration.");
        return;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    char req[128];
    sprintf(req, "POST %s CoAP\n\n%s", URI_PATH, MESSAGE);

    if (sock_udp_send(&sock, req, strlen(req), &remote) < 0) {
        puts("Error sending CoAP request");
    } else {
        puts("CoAP POST request sent!");
    }

    sock_udp_close(&sock);
}

int main(void) {
    msg_init_queue(NULL, 8);
    find_network_interface();
    init_sockets_and_send_coap();
    return 0;
}