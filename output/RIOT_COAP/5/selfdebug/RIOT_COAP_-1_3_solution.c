// Description of the code:
// This code sends a CoAP POST request using RIOT OS libraries. The request contains the payload "Hello, COAP Cloud" to a specific CoAP server with IP address 47.102.103.1 on port 5683 and URI path /coap/test.

#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <stdio.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

gcoap_request_memo_t memo;
coap_pkt_t pdu;
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    printf("Response received\n");
    // further handling of the response can be implemented here
}

int main(void) {
    printf("Starting CoAP request example\n");

    sock_udp_ep_t remote = { .port = COAP_SERVER_PORT };
    if (inet_pton(AF_INET6, COAP_SERVER_IP, &remote.addr.ipv6) <= 0) {
        printf("Error: unable to parse IPv6 address\n");
        return 1;
    }

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}