#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(unsigned req_state, coap_pkt_t* pdu, sock_udp_ep_t *remote)
{
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        puts("Timeout");
        return;
    }
    if (req_state == GCOAP_MEMO_ERR) {
        puts("Error");
        return;
    }
    printf("Response received: %u bytes\n", pdu->payload_len);
}

int main(void)
{
    uint8_t buf[128];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP);

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    gcoap_req_send(buf, pdu.payload_len, &remote, NULL, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UNDEF);

    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}