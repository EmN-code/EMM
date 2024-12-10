#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE      (128)
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_PORT (5683)
#define EMQX_URI_PATH "/coap/test"

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) memo;
    (void) remote;

    if (pdu != NULL) {
        printf("Response received: %s\n", pdu->payload);
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    /* Initialize GCoAP request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    strncpy((char *)pdu.payload, "Hello, COAP Cloud", pdu.payload_len);

    /* set up address family */
    remote.family = AF_INET;
    remote.port = EMQX_PORT;

    /* Error Check for inet_pton */
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IP\n");
        return 1;
    }

    /* header length is part of gcoap API in payload function, corrected from hdr_len and opt_len */
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, "Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);

    /* Send CoAP request */
    ssize_t result = gcoap_req_send(buf, len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Error sending request: %d\n", (int)result);
    }

    return 0;
}