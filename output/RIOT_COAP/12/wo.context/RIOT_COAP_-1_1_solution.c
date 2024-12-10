#include <stdio.h>
#include <string.h>
#include <net/gcoap.h>

#define COAP_SERVER "47.102.103.1"
#define COAP_PORT CONFIG_GCOAP_PORT
#define COAP_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static uint8_t pdu_buf[CONFIG_GCOAP_PDU_BUF_SIZE];

gcoap_request_memo_t _memo;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;
    if (pdu != NULL) {
        printf("Response received: %d\n", coap_get_code(pdu));
    }
    else {
        printf("No response from server\n");
    }
}

int main(void)
{
    /* initialize server endpoint */
    sock_udp_ep_t server = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr, COAP_SERVER);
    server.port = COAP_PORT;

    /* prepare CoAP packet */
    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, pdu_buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    len += strlen(COAP_PAYLOAD);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);

    /* send the CoAP request */
    gcoap_req_send(pdu_buf, len, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}