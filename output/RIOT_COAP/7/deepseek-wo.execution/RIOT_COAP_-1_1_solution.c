
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char resp_buf[GCOAP_PDU_BUF_SIZE];
    size_t len = gcoap_pkt_print(pdu, resp_buf, sizeof(resp_buf));
    printf("CoAP response: %s\n", resp_buf);
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    // Convert IP address string to binary form
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error: unable to parse destination address");
        return 1;
    }

    // Initialize gcoap
    gcoap_init();

    // Prepare the CoAP request
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    ssize_t payload_len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));

    // Send the CoAP request
    gcoap_req_send(&buf[0], payload_len + strlen(MESSAGE), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Keep the thread alive to receive the response
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
