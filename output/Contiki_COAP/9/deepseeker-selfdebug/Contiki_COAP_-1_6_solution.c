
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/uip-udp-packet.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_response_handler(coap_pkt_t *pkt, struct sockaddr *addr, socklen_t addr_len) {
    printf("CoAP response received\n");
}

int main(void) {
    coap_pkt_t pkt;
    uint8_t payload[] = "Hello, COAP Cloud";
    coap_init_message(&pkt, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&pkt, URI_PATH);
    coap_set_payload(&pkt, payload, sizeof(payload) - 1);

    sock_udp_ep_t local = { .family = AF_INET, .port = COAP_PORT };
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT, .addr = { .ipv4 = { 47, 102, 103, 1 } } };

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    coap_send(&sock, &pkt, &remote, coap_response_handler);

    while (1) {
        // Main loop to keep the program running
    }

    return 0;
}
