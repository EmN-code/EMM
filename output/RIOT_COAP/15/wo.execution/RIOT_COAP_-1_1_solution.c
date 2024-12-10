#include <stdio.h>
#include "net/gcoap.h"
#include "net/af.h"
#include "kernel.h"

#define COAP_METHOD_POST 0x02
#define CONFIG_GCOAP_PORT 5683

int main(void) {
    size_t pdu_buf[128];
    coap_pkt_t pdu;

    printf("Starting CoAP POST request to the server...\n");

    sockaddr_in6_t remote;
    remote.family = AF_INET6;
    remote.port = CONFIG_GCOAP_PORT;

    // Example IPv4 address embedded in IPv6
    inet_pton(AF_INET6, "::FFFF:47.102.103.1", &remote.addr.ipv6);

    gcoap_req_init(&pdu, pdu_buf, sizeof(pdu_buf), COAP_METHOD_POST, "/coap/test");

    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    ssize_t res = gcoap_req_send(&pdu_buf[0], pdu.payload_len, (sock_udp_ep_t*)&remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res > 0) {
        printf("CoAP POST request sent successfully\n");
    } else {
        printf("Failed to send CoAP POST request\n");
    }

    return 0;
}