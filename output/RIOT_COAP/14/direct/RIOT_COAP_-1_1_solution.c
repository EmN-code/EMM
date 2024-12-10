#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/coap.h"
#include "kernel.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET6,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = SERVER_PORT
    };

    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse IPv6 address");
        return 1;
    }

    coap_pkt_t pdu;
    uint8_t buf[128];

    unsigned msg_type = COAP_TYPE_CON;
    size_t len = coap_build_put(&pdu, buf, sizeof(buf), msg_type, NULL, 0);
    coap_hdr_set_uri_path(&pdu, URI_PATH);
    coap_pkt_append_payload_marker(&pdu);
    coap_pkt_append_payload(&pdu, (uint8_t *)MESSAGE, strlen(MESSAGE));

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    if (sock_udp_send(&sock, buf, len, &server) < 0) {
        puts("Error: unable to send message");
        return 1;
    }

    printf("CoAP message sent to %s on port %d\n", SERVER_ADDR, SERVER_PORT);

    while (1) {
        xtimer_sleep(60);
    }

    return 0;
}