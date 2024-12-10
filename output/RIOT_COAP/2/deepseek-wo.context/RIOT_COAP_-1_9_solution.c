
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        char resp_phrase[32];
        gcoap_response_phrase(pdu->hdr->code, resp_phrase, sizeof(resp_phrase));
        printf("Received response: %s\n", resp_phrase);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int res;

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize the CoAP request
    res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (res < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    // Set the payload and content format
    coap_pkt_set_payload(&pdu, MESSAGE, strlen(MESSAGE));
    coap_pkt_set_content_format(&pdu, COAP_FORMAT_TEXT_PLAIN);

    // Send the CoAP request
    res = gcoap_req_send(buf, coap_pkt_get_total_size(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        puts("Failed to send CoAP request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}
