
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char *resp_phrase = (char *)gcoap_response_phrase(pdu->hdr->code);
    printf("Received CoAP response: %s\n", resp_phrase);
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    int result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    len += 17;

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}