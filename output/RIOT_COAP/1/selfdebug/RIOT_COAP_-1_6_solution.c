#include "net/sock/udp.h"
#include "net/sock/dtls.h"
#include "net/ipv4/addr.h"
#include <string.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE (128)

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) memo;
    (void) remote;

    const uint8_t *payload;
    unsigned code;
    if (pdu != NULL) {
        payload = coap_get_payload(pdu, &code);
        printf("Response code: %d\n", code);
        printf("Payload: %s\n", payload);
    }
    else {
        printf("Timeout or error while requesting\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .port = CONFIG_GCOAP_PORT };
    coap_pkt_t pdu;

    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    /* initialize CoAP request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    coap_hdr_set_type(buf, GCOAP_MSG_CON);
    memcpy(pdu.payload, "Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);

    /* send packet */
    if (gcoap_req_send(buf, (pdu.payload - buf) + sizeof("Hello, COAP Cloud") - 1, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("gcoap_req_send failed\n");
    }
    else {
        printf("Request sent\n");
    }

    return 0;
}