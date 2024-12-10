
#include <stdio.h>
#include <netinet/in.h>
#include "gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    size_t len = sizeof(buf);
    gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
    coap_opt_add_format(pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);
    printf("CoAP response: %s\n", (char *)pdu->payload);
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    local.port = GCOAP_PORT;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP);

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_pkt_set_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17);

    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
