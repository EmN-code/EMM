#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "net/gcoap.h"
#include "net/gnrc/netif.h"

#define GCOAP_PDU_BUF_SIZE (128U)
#define REMOTE_ADDR_IPV6 "47.102.103.1"
#define REMOTE_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static char _pdu_buf[GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote)
{
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP_RECEIVED) {
        printf("Response received, code: %u\n", coap_get_code(pdu));
    }
    else {
        printf("No response or request timeout\n");
    }
}

int main(void) {
    uint8_t buf[64];
    size_t len;
    sock_udp_ep_t remote;
    coap_pkt_t pdu;

    /* Set remote to server address and port */
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = REMOTE_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, REMOTE_ADDR_IPV6);

    /* Initialize the CoAP PDU */
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    len += strlen("Hello, COAP Cloud");
    coap_hdr_set_payload_len(pdu.hdr, len - gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH));

    /* Send CoAP request */
    if (gcoap_req_send(_pdu_buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error sending CoAP request\n");
        return 1;
    }

    puts("CoAP Request Sent\n");
    return 0;
}