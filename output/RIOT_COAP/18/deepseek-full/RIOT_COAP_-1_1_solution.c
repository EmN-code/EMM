
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char resp_buf[GCOAP_PDU_BUF_SIZE];
    size_t len = gcoap_pdu_parse(pdu->payload, pdu->payload_len, resp_buf, sizeof(resp_buf));
    if (len > 0) {
        printf("Received CoAP response: %s\n", resp_buf);
    }
    else {
        puts("Error parsing CoAP response");
    }
}

static int send_coap_request(void)
{
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;

    remote.port = COAP_SERVER_PORT;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    if (sock_udp_name2ep(&remote, COAP_SERVER_IP, NULL) < 0) {
        puts("Error resolving server address");
        return -1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    len += strlen(COAP_MESSAGE);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending CoAP request");
        return -1;
    }

    return 0;
}

int main(void)
{
    puts("Starting CoAP client");

    if (gcoap_init() < 0) {
        puts("Error initializing gcoap");
        return -1;
    }

    while (1) {
        if (send_coap_request() < 0) {
            puts("Failed to send CoAP request");
        }

        xtimer_sleep(10);  // Send request every 10 seconds
    }

    return 0;
}
