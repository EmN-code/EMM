
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;
    char buf[GCOAP_PDU_BUF_SIZE];
    gcoap_resp_init(pdu, buf, sizeof(buf), COAP_CODE_CONTENT);
    coap_opt_add_format(pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);
    printf("CoAP response: %s\n", (char *)pdu->payload);
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid address");
        return 1;
    }

    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    pdu.payload_len = strlen(MESSAGE);

    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
