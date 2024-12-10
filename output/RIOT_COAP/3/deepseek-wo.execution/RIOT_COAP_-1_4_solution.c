
#include <stdio.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void)
{
    sock_udp_ep_t server_ep = SOCK_IPV4_EP_ANY;
    server_ep.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &server_ep.addr.ipv4.u32) <= 0) {
        printf("Invalid address\n");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD) - 1);
    len += sizeof(PAYLOAD) - 1;

    gcoap_req_send(buf, len, &server_ep, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
