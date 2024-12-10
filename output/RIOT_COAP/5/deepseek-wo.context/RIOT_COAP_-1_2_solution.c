
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
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        char *resp = gcoap_response_phrase(pdu->hdr->code);
        printf("Received CoAP response: %s\n", resp);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = COAP_SERVER_PORT;

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid server IP address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    int res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (res < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);

    res = coap_opt_add_format(&pdu, COAP_FORMAT_TEXT_PLAIN);
    if (res < 0) {
        puts("Failed to add format option");
        return 1;
    }

    res = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (res < 0) {
        puts("Failed to finish options");
        return 1;
    }

    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len = payload_len;

    gcoap_req_send(buf, res, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
