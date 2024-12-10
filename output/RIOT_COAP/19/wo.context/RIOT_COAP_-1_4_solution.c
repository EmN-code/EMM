#include <stdio.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

// This handler will be called when a response is received
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;

    // Convert the server IP string to a binary form
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to encode IPv4 address\n");
        return -1;
    }
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    // Initialize the CoAP request
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (len < 0) {
        puts("Error: unable to initialize CoAP request\n");
        return -1;
    }

    // Add payload to the CoAP request
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    // Finish the options in the CoAP request
    int finish_res = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    if (finish_res < 0) {
        puts("Error: unable to finish CoAP options\n");
        return -1;
    }

    // Send the CoAP request
    res = gcoap_req_send(buf, finish_res, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error: failed to send CoAP request\n");
        return -1;
    }
    printf("CoAP request sent: %ld bytes\n", res);

    return 0;
}