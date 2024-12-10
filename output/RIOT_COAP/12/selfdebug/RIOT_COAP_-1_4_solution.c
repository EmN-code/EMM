#include "net/sock/udp.h"
#include "net/gcoap.h"
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

gcoap_request_memo_t req_memo;

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) remote; // Unused parameter
    // Handle response
    printf("Response received\n");
    if (pdu) {
        printf("Response code: %d\n", gcoap_response_code(pdu));
    }
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t server_ep;

    // Convert server IP address
    if (inet_pton(AF_INET, SERVER_IP, &server_ep.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return -1;
    }

    server_ep.family = AF_INET;
    server_ep.port = SERVER_PORT;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Set payload for request
    size_t payload_len = strlen(MESSAGE);
    memcpy(coap_payload(&pdu), MESSAGE, payload_len);
    coap_hdr_set_type(&pdu, COAP_TYPE_NON); // CoAP message type

    // Send CoAP request
    if (gcoap_req_send(buf, coap_payload_len(&pdu), &server_ep, _resp_handler, &req_memo, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error: unable to send request\n");
        return -1;
    }

    printf("Request sent\n");
    return 0;
}