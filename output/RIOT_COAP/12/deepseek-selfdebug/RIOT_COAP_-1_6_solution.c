
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = COAP_SERVER_PORT;

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    pdu.payload_len = strlen(COAP_MESSAGE);

    gcoap_req_send(&buf[0], pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}