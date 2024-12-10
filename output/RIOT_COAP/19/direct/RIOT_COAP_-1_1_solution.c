#include "net/nanocoap.h"
#include "net/coap.h"
#include "msg.h"
#include "net/sock/udp.h"
#include "stdio.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define BUFFER_SIZE 128

int main(void) {
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = SERVER_PORT
    };

    // Parse the IP address
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse destination address");
        return 1;
    }

    // Create socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create socket");
        return 1;
    }

    // Buffer for request packet
    char buffer[BUFFER_SIZE];
    coap_pkt_t pdu;
    coap_builder_init(&pdu);
    coap_build_hdr(&pdu, COAP_TYPE_CON, NULL, 0, COAP_METHOD_POST, 0);
    coap_set_header_uri_path(&pdu, URI_PATH);
    coap_add_payload(&pdu, (const uint8_t *)"Hello, COAP Cloud", 16);

    // Build the CoAP packet
    ssize_t len = coap_build(buffer, &pdu, BUFFER_SIZE);

    if (len < 0) {
        puts("Error: unable to build CoAP packet");
        return 1;
    }

    // Send the packet
    if (sock_udp_send(&sock, buffer, len, &remote) < 0) {
        puts("Error: unable to send packet");
        return 1;
    }

    puts("CoAP POST request sent");
    return 0;
}