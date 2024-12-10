#include "thread_config.h"
#include "net/nanocoap/cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gcoap.h"
#include "net/sock/udp.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"

static sock_udp_ep_t remote;
static char *message = "Hello, COAP Cloud";

static void _resp_handler(const gcoap_request_memo_t *memo,
                          coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_ACK) {
        puts("Response received");
    }
    else {
        puts("No response received");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len;

    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP);

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    memcpy(pdu.payload, message, strlen(message));
    len += strlen(message);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while(1) {
        thread_yield();
    }

    return EXIT_SUCCESS;
}