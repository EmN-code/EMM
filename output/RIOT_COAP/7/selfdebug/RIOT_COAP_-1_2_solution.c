#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/inet/addr.h>

#define SERVER_ADDR    "47.102.103.1"  // Server IP address
#define SERVER_PORT    CONFIG_GCOAP_PORT // CoAP default port 5683
#define URI_PATH       "/coap/test"
#define PAYLOAD        "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received from server\n");
    // Handle response here
}

int main(void) {
    // Create endpoint structure
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    if (inet_pton(AF_INET6, SERVER_ADDR, &remote.addr.ipv6) != 1) {
        printf("Error: unable to parse IPv6 address\n");
        return -1;
    }

    // Initialize a CoAP packet buffer
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    size_t len = coap_pkt_append_payload(&pdu, (uint8_t *)PAYLOAD, sizeof(PAYLOAD) - 1);
    if (!len) {
        printf("Error: unable to append payload\n");
        return -1;
    }

    // Send the CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error: unable to send CoAP request\n");
        return -1;
    }

    printf("CoAP POST request sent to %s%s\n", SERVER_ADDR, URI_PATH);
    return 0;
}