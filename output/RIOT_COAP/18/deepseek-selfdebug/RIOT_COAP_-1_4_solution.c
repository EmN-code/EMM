
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;
    char *resp = gcoap_resp_init(pdu, NULL, COAP_CODE_CONTENT);
    if (resp) {
        printf("Received response: %s\n", resp);
    } else {
        printf("Failed to initialize response\n");
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}