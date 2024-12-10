
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char *payload = (char *)pdu->payload;
    printf("Received response: %s\n", payload);
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    gcoap_req_send(&buf[0], pdu.payload_len + pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    xtimer_sleep(1); // Wait for response

    return 0;
}
