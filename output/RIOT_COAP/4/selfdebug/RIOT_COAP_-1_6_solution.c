#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/gcoap.h"
#include <stdio.h>

#define COAP_SERVER_IP     "47.102.103.1"
#define COAP_SERVER_PORT   5683
#define URI_PATH           "/coap/test"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    }
    else if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout\n");
    }
}

int main(void) {
    // Initialize socket address for CoAP server
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;

    coap_pkt_t pdu;
    ssize_t len;
    
    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Error: Invalid IP address\n");
        return -1;
    }

    // Initialize the CoAP packet
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Error: Failed to initialize CoAP request\n");
        return -1;
    }

    // Set the payload data
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Send the CoAP request
    if (gcoap_req_send(buf, len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: Failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    
    return 0;
}