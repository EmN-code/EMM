
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
    (void)memo;
    (void)remote;

    if (pdu) {
        printf("CoAP status: %u\n", (unsigned)pdu->hdr->code);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Initialize the CoAP request
    result = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (result < 0) {
        puts("Error: failed to initialize CoAP request");
        return 1;
    }

    // Set the payload and content format
    coap_pkt_set_payload(&pdu, (uint8_t *)MESSAGE, strlen(MESSAGE));
    coap_pkt_set_content_format(&pdu, COAP_FORMAT_TEXT_PLAIN);

    // Get the total size of the CoAP packet
    size_t len = coap_pkt_get_total_size(&pdu);

    // Send the CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
