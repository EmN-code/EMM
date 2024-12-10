#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define COAP_SERVER_IPV4 "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("gcoap: timeout");
        return GCOAP_MEMO_TIMEOUT;
    }
    if (memo->state == GCOAP_MEMO_ERR) {
        printf("gcoap: error %d\n", memo->state);
        return GCOAP_MEMO_ERR;
    }
    printf("gcoap: received response from [\%s]:\%u\n",
           remote->addr.ipv4, remote->port);

    return 0;
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr, COAP_SERVER_IPV4);
    remote.port = COAP_SERVER_PORT;

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy((char *)pdu.payload, "Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);
    len += sizeof("Hello, COAP Cloud") - 1;

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending CoAP request");
    }

    return 0;
}