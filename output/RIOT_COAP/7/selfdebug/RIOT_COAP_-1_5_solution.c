#include "net/ipv6/addr.h"
#include "net/ipv4/addr.h"
#include <stdio.h>
#include "net/gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

enum returnCode { FAILURE = -1, SUCCESS = 0 };

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Timeout received");
        return;
    }

    if (pdu->code == COAP_CODE_205) {
        puts("Received ACK");
    }
    else {
        puts("Unexpected code");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Resolve the server address
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr) != 1) {
        puts("Error: Unable to resolve server address");
        return FAILURE;
    }
    remote.port = COAP_SERVER_PORT;

    // Initialize the packet
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (len < 0) {
        puts("Error initializing request packet");
        return FAILURE;
    }

    // Copy payload into the packet
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send the request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Failed to send CoAP request");
        return FAILURE;
    }

    puts("CoAP request sent");
    return SUCCESS;
}