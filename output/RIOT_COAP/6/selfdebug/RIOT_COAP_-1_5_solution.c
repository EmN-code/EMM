#include <net/sock/udp.h>
#include <net/gnrc/gcoap.h>
#include <stdio.h>
#include <string.h>

#define CONFIG_GCOAP_PORT 5683
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.port = CONFIG_GCOAP_PORT;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    
    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    puts("CoAP request sent\n");

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) remote;
    
    if (!memo || !pdu) {
        puts("Error in request\n");
        return;
    }
    printf("Response received: Code: %u\n", pdu->code);
}