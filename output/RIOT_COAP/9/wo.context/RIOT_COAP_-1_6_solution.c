#include <stdio.h>              // Include the standard input-output library
#include <string.h>            // Include the string manipulation library
#include <net/ipv4/addr.h>    // Include IPv4 address utility functions
#include <net/gcoap.h>        // Include the gcoap module for sending CoAP messages

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_PORT 5683
#define SERVER_IP "47.102.103.1"
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_STATE_COMPLETED) {
        printf("Response received successfully\n");
    }
    else {
        printf("Request not completed. State: %u\n", memo->state);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int result;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        printf("Error: Failed to convert IP address\n");
        return -1;
    }
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;

    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: Request not sent\n");
        return -1;
    }

    printf("Request sent successfully\n");
    return 0;
}