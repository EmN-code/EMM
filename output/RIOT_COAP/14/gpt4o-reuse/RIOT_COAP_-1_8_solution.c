
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (pdu) {
        printf("Received CoAP response\n");
    } else {
        printf("No response received\n");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    int res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (res < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    gcoap_add_payload(&pdu, (uint8_t *)MESSAGE, strlen(MESSAGE));

    gcoap_req_send(buf, res, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        gcoap_process();
    }

    return 0;
}
