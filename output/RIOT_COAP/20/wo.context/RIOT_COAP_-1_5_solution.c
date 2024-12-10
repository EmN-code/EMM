#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

// Allocate a buffer for the CoAP PDU
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

// Response handler
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    // Endpoint structure
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    // CoAP packet structure
    coap_pkt_t pdu;
    memset(&pdu, 0, sizeof(pdu));
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    // Set payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send the request
    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}