#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <msg.h>

#define COAP_SERVER_IP       "47.102.103.1"
#define COAP_EMQX_PORT       (5683)
#define EMQX_URI_PATH        "/coap/test"
#define PAYLOAD              "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received: ");
        if (pdu->payload_len) {
            printf("%.*s\n", pdu->payload_len, (char *)pdu->payload);
        }
        else {
            printf("no payload\n");
        }
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    /* Convert IP address */
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_EMQX_PORT;

    /* Initialize CoAP request */
    ssize_t bytes = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);

    /* Add payload to PDU */
    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD));
    coap_hdr_set_payload_len(pdu.hdr, sizeof(PAYLOAD));

    /* Send CoAP request */
    gcoap_req_send(buf, bytes + sizeof(PAYLOAD), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}