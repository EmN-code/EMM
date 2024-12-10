
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

    char *resp_buf = (char *)pdu->payload;
    size_t resp_len = pdu->payload_len;

    printf("Received CoAP response: %.*s\n", (int)resp_len, resp_buf);
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    remote.port = COAP_SERVER_PORT;
    remote.family = AF_INET;

    // Convert IP address string to binary form
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, COAP_SERVER_IP) == NULL) {
        puts("Error: invalid server IP address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Add payload
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    len += strlen(COAP_MESSAGE);

    // Send CoAP request
    gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Keep the thread alive to receive the response
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
