
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    if (pdu->hdr->code == COAP_CODE_CONTENT) {
        printf("Received response: %s\n", (char *)pdu->payload);
    } else {
        printf("Error in response\n");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    int result;

    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 16);
    pdu.payload_len = 16;

    gcoap_req_send(&buf[0], pdu.payload_len + ((uint8_t *)pdu.payload - buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}