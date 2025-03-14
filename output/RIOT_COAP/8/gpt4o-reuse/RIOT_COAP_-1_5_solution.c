
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (pdu->hdr->code == COAP_CODE_CONTENT) {
        printf("Received response: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    } else {
        printf("Error in response\n");
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;

    remote.port = SERVER_PORT;
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        gcoap_event_loop();
    }

    return 0;
}
