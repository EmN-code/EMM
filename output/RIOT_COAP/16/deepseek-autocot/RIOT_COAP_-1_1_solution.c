
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char resp[GCOAP_PDU_BUF_SIZE];
    size_t len = gcoap_pdu_parse(pdu->payload, pdu->payload_len, resp, sizeof(resp));
    if (len > 0) {
        printf("Received CoAP response: %s\n", resp);
    }
    else {
        puts("Error parsing CoAP response");
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error parsing server IP address");
        return 1;
    }

    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    ssize_t payload_len = coap_payload_start(&pdu);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    gcoap_req_send(buf, payload_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
