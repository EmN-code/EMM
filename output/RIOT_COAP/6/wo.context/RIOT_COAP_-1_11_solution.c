#include <string.h>
#include <stdio.h>
#include <net/gcoap.h>
#include <net/inet.h>

#define SERVER_IPV4 "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) memo;
    (void) remote;
    if (pdu->code == COAP_CODE_201_CREATED) {
        printf("Success: Resource created\n");
    } else {
        printf("Error: %u\n", coap_get_code(pdu));
    }
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IPV4, &remote.addr.ipv4);
    
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    size_t len = pdu.hdr_len + strlen(PAYLOAD);
    
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    return 0;
}