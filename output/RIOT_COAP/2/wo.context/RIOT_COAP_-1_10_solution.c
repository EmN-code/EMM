#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define COAP_PORT 5683
#define COAP_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        printf("Response received: %d\n", coap_get_code(pdu));
    }
    else {
        printf("No response\n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.port = COAP_PORT;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    gcoap_finish(&pdu, strlen(PAYLOAD), COAP_CODE_EMPTY);

    if (gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
    }
    
    return 0;
}