#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) 
{
    (void)remote;
    (void)memo;
    
    if (pdu != NULL) {
        printf("CoAP Response received!\n");
    }
    else {
        printf("No CoAP response received.\n");
    }
}

int main(void) 
{
    coap_pkt_t pdu;
    int res;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;

    res = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        printf("Error: could not parse gateway address\n");
        return 1;
    }

    res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (res < 0) {
        printf("Failed to initialize request\n");
        return 1;
    }

    if (strlen(COAP_MESSAGE) > pdu.payload_left) {
        printf("Error: Payload too large\n");
        return -1;
    }

    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    int len = gcoap_finish(&pdu, strlen(COAP_MESSAGE), GCOAP_TYPE_CON);

    if (len <= 0) {
        printf("Failed to finish CoAP request\n");
        return 1;
    }

    ssize_t bytes_sent = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");

    return 0;
}