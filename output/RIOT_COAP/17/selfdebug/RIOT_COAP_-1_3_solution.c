#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define COAP_URI_PATH   "/coap/test"
#define SERVER_IP       "47.102.103.1"
#define COAP_PORT       5683
#define PAYLOAD         "Hello, COAP Cloud"

gcoap_request_memo_t _memo;
sock_udp_ep_t remote;
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response handler called\n");
    if (pdu == NULL) {
        printf("No response received\n");
        return;
    }
    printf("Response received\n");
    // Handle your response data here...
}

int main(void) {
    // Initialize remote endpoint
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;
    
    // Prepare CoAP request
    coap_pkt_t pdu;
    size_t len;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Send request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("Request sent\n");

    return 0;
}