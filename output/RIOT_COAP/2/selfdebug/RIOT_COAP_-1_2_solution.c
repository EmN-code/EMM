#include "net/gcoap.h"
#include "net/af.h"
#include "msg.h"
#include "shell.h"
#include "thread.h"
#include <stdio.h>
#include <string.h>

#define SERVER_EP "coap://47.102.103.1:5683"
#define URI_PATH "/coap/test"

#define BUFFER_SIZE        (CONFIG_GCOAP_PDU_BUF_SIZE)
#define PAYLOAD            "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_RESP)
    {
        printf("Response received: %s\n", (char *)pdu->payload);
    }
    else
    {
        printf("No response received\n");
    }
}

int main(void)
{
    uint8_t buf[BUFFER_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, BUFFER_SIZE, COAP_METHOD_POST, URI_PATH);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    sock_udp_ep_t server_ep;
    server_ep.family = AF_INET6;
    server_ep.netif = SOCK_ADDR_ANY_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)&server_ep.addr.ipv6, SERVER_EP);
    server_ep.port = CONFIG_GCOAP_PORT;

    gcoap_req_send(buf, len, &server_ep, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}