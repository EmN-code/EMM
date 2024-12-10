#include <string.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE      (128)
#define COAP_PORT 5683
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

#include "net/gcoap.h"

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response - code implementation for handling logic if necessary
    (void)memo;
    (void)pdu;
    (void)remote;
}

int main(void) {
    // Define variables
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Prepare remote endpoint
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_PORT;
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Unable to parse IP address\n");
        return -1;
    }

    // Prepare CoAP packet
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: Unable to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent successfully to %s\n", EMQX_SERVER_IP);
    return 0;
}