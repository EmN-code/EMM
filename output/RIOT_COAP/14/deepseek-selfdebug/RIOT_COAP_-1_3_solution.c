
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
        printf("CoAP status: %d\n", (int)coap_get_code(pdu));
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
        puts("Error converting IP address");
        return 1;
    }

    // Initialize the CoAP request
    result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (result < 0) {
        puts("Error initializing CoAP request");
        return 1;
    }

    // Set the payload
    size_t payload_len = strlen(MESSAGE);
    memcpy(pdu.payload, MESSAGE, payload_len);
    pdu.payload_len = payload_len;

    // Finish the CoAP options
    result = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    if (result < 0) {
        puts("Error finishing CoAP options");
        return 1;
    }

    // Send the CoAP request
    result = gcoap_req_send(buf, result, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}
