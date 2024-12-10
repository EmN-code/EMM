#include "net/nanocoap/cache.h"
#include <stdio.h>
#include <stdlib.h>
#include "gcoap.h"
#include "kernel_types.h"
#include "net/sock/udp.h"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = 5683
    };

    /* Set up the IP address of the CoAP server */
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, "47.102.103.1");

    /* Initialize the CoAP request. */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    /* Copy the payload to the PDU */
    memcpy(pdu.payload, "Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);
    pdu.payload_len = sizeof("Hello, COAP Cloud") - 1;

    /* Set up message ID and token if needed */
    coap_hdr_set_id(&pdu, 0x1234);     /* example ID */
    coap_hdr_set_token(&pdu, 0x4567);  /* example Token */

    /* Send the request */
    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending CoAP request\n");
    }
    else {
        printf("CoAP request sent\n");
    }

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP_RECEIVED) {
        printf("Response received: %u bytes\n", pdu->payload_len);
        if (pdu->payload_len) {
            printf("%.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
        }
    }
}