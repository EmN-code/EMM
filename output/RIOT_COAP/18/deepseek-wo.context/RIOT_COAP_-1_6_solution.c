
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>  // Include this header for inet_pton
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char *resp = gcoap_response_phrase(coap_get_code(pdu));
    printf("CoAP status: %s\n", resp);
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;

    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        puts("Error: cannot initialize CoAP request");
        return 1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
