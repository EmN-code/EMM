#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "string.h"
#include "ztimer.h"
#include "timex.h"
#include "msg.h"
#include "arpa/inet.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

// Response handler function
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Error: request timed out\n");
        return;
    }

    printf("Response received: %u bytes\n", (unsigned)pdu->payload_len);
    printf("Payload: %.*s\n", pdu->payload_len, (char *)pdu->payload);
}

int main(void) {
    // Initialize the CoAP PDU buffer
    uint8_t buf[128];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Configure the remote server's IP address and port
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }

    // Initialize the CoAP request
    unsigned code = COAP_METHOD_POST;
    int len = gcoap_req_init(&pdu, buf, sizeof(buf), code, URI_PATH);
    if (len < 0) {
        printf("Failed to initialize request\n");
        return -1;
    }

    // Add payload to PDU
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Failed to send request\n");
        return -1;
    }

    printf("CoAP request sent successfully\n");
    
    // Run the event loop to wait for response
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
