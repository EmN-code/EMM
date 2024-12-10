#include <stdio.h>
#include "net/gcoap.h"
#include "net/gnrc/netif.h"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET6, .port = CONFIG_GCOAP_PORT };

    size_t len;
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    const char *message = "Hello, COAP Cloud";
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    memcpy(pdu.payload, message, strlen(message));
    len += strlen(message);

    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, "47.102.103.1");

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    puts("CoAP POST request sent successfully!");

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}