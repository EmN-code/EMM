
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

    if (pdu) {
        char *resp_phrase = gcoap_response_phrase(pdu->hdr->code);
        printf("Received CoAP response: %s\n", resp_phrase);
    } else {
        printf("No response received\n");
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid server IP address\n");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    const char *payload = "Hello, COAP Cloud";
    len = coap_opt_finish(&pdu, len, COAP_FORMAT_TEXT, payload, strlen(payload));
    if (len < 0) {
        printf("Failed to finish CoAP request\n");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
