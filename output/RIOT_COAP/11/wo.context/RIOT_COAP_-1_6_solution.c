#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/coap.h>
#include <stdio.h>  // For printf

#define COAP_SERVER_IP   "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH         "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    // Define buffer for CoAP PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Define remote endpoint
    sock_udp_ep_t remote;
    remote.family = AF_INET6;  // IPv6
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;

    // Convert IP address
    int res = inet_pton(AF_INET6, COAP_SERVER_IP, &remote.addr.ipv6);
    if (res <= 0) {
        printf("Error in IP conversion\n");
        return -1;
    }

    // Initialize CoAP PDU
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    // Set payload
    memcpy(pdu.payload, "Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);
    pdu.payload_len = sizeof("Hello, COAP Cloud") - 1;

    // Send request
    ssize_t bytes_sent = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote,
                           _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");

    return 0;
}