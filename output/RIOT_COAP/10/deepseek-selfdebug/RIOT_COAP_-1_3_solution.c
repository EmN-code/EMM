
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

    if (pdu->hdr->code == COAP_CODE_CONTENT) {
        printf("Received response: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    } else {
        printf("Error in response\n");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Add payload
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len = payload_len;

    // Send CoAP request
    ssize_t len = gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");

    return 0;
}