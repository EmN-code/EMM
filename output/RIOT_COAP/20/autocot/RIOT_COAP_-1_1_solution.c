#include <net/nanocoap/cache.h>
#include <gcoap.h>
#include <net/gnrc/netif.h>
#include <net/gnrc/ipv6.h>
#include <net/sock/udp.h>
#include <string.h>

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (pdu == NULL) {
        printf("Error sending CoAP request\n");
    }
    else {
        printf("CoAP response received\n");
    }
}

int main(void)
{
    gcoap_init();

    uint8_t buffer[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDRESS);

    size_t len = gcoap_req_init(&pdu, buffer, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, sizeof(COAP_MESSAGE));
    gcoap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    len += sizeof(COAP_MESSAGE);

    gcoap_req_send(buffer, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}