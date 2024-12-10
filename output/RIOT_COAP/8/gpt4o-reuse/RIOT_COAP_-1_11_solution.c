
#include <stdio.h>
#include <netinet/in.h>
#include "gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char buf[GCOAP_PDU_BUF_SIZE];
    size_t len = gcoap_pdu_parse(pdu, buf, sizeof(buf));
    if (len > 0) {
        printf("Received CoAP response: %s\n", buf);
    } else {
        puts("Error parsing CoAP response");
    }
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    sock_udp_t sock;

    local.port = GCOAP_PORT;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    ssize_t sent = sock_udp_send(&sock, buf, len, &remote);
    if (sent < 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    while (1) {
        sock_udp_ep_t remote_ep;
        ssize_t resp_len = sock_udp_recv(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT, &remote_ep);
        if (resp_len > 0) {
            coap_pkt_t resp_pdu;
            if (gcoap_pdu_parse(&resp_pdu, buf, resp_len) > 0) {
                _resp_handler(NULL, &resp_pdu, &remote_ep);
            }
        }
    }

    return 0;
}
