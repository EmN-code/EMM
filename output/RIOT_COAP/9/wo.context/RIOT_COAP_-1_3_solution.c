#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/inet.h>  // For inet_pton
#include <net/sock/udp.h>  // For UDP socket functions
#include <gcoap.h>  // For CoAP functions

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"

static sock_udp_ep_t remote;
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
coap_pkt_t pdu;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Received response\n");
}

int main(void) {
    // Initialize server address structure
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    size_t len = sizeof(buf);
    memcpy(pdu.payload, COAP_MSG, strlen(COAP_MSG));
    len += strlen(COAP_MSG);

    // Send the CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    return 0;
}