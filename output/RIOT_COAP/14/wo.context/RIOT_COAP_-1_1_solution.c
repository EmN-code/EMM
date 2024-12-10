#include <string.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IPV6_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"

#define COAP_MSG "Hello, COAP Cloud"

static sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    printf("Response received, code: %u\n", coap_get_code(pdu));
    if (pdu->payload_len) {
        printf("Payload: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IPV6_ADDR);

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_NCONFIRMABLE);
    len += coap_opt_add_format(&pdu, COAP_FORMAT_NONE);
    len += coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    memcpy(pdu.payload, COAP_MSG, sizeof(COAP_MSG));
    pdu.payload_len = sizeof(COAP_MSG);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
    }
    else {
        printf("CoAP request sent\n");
    }

    return 0;
}
