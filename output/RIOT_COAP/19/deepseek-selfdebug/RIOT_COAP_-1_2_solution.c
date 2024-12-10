
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
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

    if (pdu) {
        printf("Received CoAP response: %d.%02d\n", coap_get_code_class(pdu), coap_get_code_detail(pdu));
    } else {
        printf("No response received\n");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    ipv4_addr_t server_addr;

    if (ipv4_addr_from_str(&server_addr, SERVER_IP) == NULL) {
        printf("Error parsing server IP address\n");
        return 1;
    }

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.addr.ipv4 = server_addr;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_pkt_set_payload(&pdu, (uint8_t *)MESSAGE, strlen(MESSAGE));

    ssize_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    if (len <= 0) {
        printf("Failed to create CoAP request\n");
        return 1;
    }

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");

    return 0;
}
