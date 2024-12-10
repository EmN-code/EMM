#include <stdio.h>
#include <unistd.h>
#include "net/gcoap.h"
#include "net/af.h"
#include "net/sock/udp.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    printf("Response received: %u bytes\n", coap_get_total_len(pdu->pdu));
}

int main(void) {
    uint8_t buffer[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;

    printf("CoAP client example\n");

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP);

    while (1) {
        gcoap_req_init(&pdu, buffer, sizeof(buffer), COAP_METHOD_POST, COAP_URI_PATH);
        coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
        memcpy(coap_payload(pdu.hdr), "Hello, COAP Cloud", 18);
        coap_set_payload_len(&pdu, 18);

        res = gcoap_req_send(buffer, pdu.buf->pos, &remote, _resp_handler, NULL);
        if (res < 0) {
            printf("Error sending request: %d\n", (int)res);
            xtimer_sleep(1);
        } else {
            printf("Request sent\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}