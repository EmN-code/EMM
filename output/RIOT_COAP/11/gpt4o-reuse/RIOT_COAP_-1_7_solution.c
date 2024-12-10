
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
    unsigned char buf[GCOAP_PDU_BUF_SIZE];
    ssize_t len = gcoap_get_payload(pdu, &buf);

    if (len > 0) {
        printf("Received: %.*s\n", (int)len, buf);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    ssize_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    if (len < 0) {
        puts("Error in request setup");
        return 1;
    }

    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    len = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (len < 0) {
        puts("Failed to send request");
        return 1;
    }

    puts("Request sent");

    return 0;
}
