#include "net/gcoap.h"
#include "arpa/inet.h"
#include "net/ipv6/addr.h"
#include <stdio.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define COAP_METHOD_POST 2

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received.\n");
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        printf("Error: invalid IP address\n");
        return FAILURE;
    }
    
    remote.port = CONFIG_GCOAP_PORT;
    
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    const char* payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    gcoap_req_send(&buf[0], strlen(payload), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    printf("CoAP request sent.\n");
    return SUCCESS;
}