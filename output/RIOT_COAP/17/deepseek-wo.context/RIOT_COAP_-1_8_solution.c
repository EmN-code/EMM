
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char resp_buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t resp_len = gcoap_response(pdu, (uint8_t *)resp_buf, sizeof(resp_buf), COAP_CODE_CONTENT);

    if (resp_len > 0) {
        printf("Received CoAP response: %s\n", resp_buf);
    } else {
        printf("No response received\n");
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = COAP_SERVER_PORT;

    int result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_pkt_set_payload(&pdu, (uint8_t *)payload, strlen(payload));

    ssize_t len = coap_pkt_finalize(&pdu, COAP_TYPE_CON);
    if (len < 0) {
        printf("Failed to finalize CoAP packet\n");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
