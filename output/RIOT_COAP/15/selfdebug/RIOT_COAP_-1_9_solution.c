#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <gcoap.h>
#include <stdio.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;

    if (!memo) {
        puts("Error receiving response");
        return;
    }
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Response timeout");
        return;
    }
    printf("Response code: %u.%u\n",
           coap_get_code_class(pdu),
           coap_get_code_detail(pdu));
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Initialize the destination address
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.port = COAP_SERVER_PORT;

    // Create CoAP PDU
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, sizeof(COAP_PAYLOAD));
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);

    // Set payload length
    pdu.payload_len = sizeof(COAP_PAYLOAD);

    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("Error sending request");
        return 1;
    }
    puts("Request sent");

    return 0;
}