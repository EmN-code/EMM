#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/ipv4/addr.h"
#include <string.h>
#include <stdio.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 2
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683

static const char *PAYLOAD = "Hello, COAP Cloud";

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Initialize the remote endpoint structure
    remote.family = AF_INET6;  // Assuming IPv6 usage
    remote.port = EMQX_SERVER_PORT;
    if (inet_pton(AF_INET6, EMQX_SERVER_IP, &remote.addr.ipv6) == 0) {
        printf("Error parsing IP address\n");
        return -1;
    }

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send the request
    ssize_t len = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len <= 0) {
        printf("Error sending request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}