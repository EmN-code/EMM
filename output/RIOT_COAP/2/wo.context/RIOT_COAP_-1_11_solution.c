#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("CoAP request timeout");
        return -1;
    }
    if ((pdu == NULL) || (pdu->hdr_len == 0)) {
        puts("Invalid CoAP response received");
        return -1;
    }
    printf("Received CoAP response: Code: %u\n", coap_get_code(pdu));
    return 0;
}

int main(void) {
    sock_udp_ep_t remote;
    gcoap_request_memo_t memo;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize remote address
    remote.family = AF_INET;
    remote.port = COAP_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse remote address");
        return -1;
    }

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Send request
    ssize_t result = gcoap_req_send(buf, pdu.payload_len + (pdu.payload - pdu.hdr), &remote, _resp_handler, &memo, COAP_TYPE_CON);
    if (result <= 0) {
        puts("Error sending CoAP request");
        return -1;
    }

    puts("CoAP request sent");

    // Wait for and process response
    while (memo.state != GCOAP_MEMO_COMPLETE) {
        // In practice, you might want to implement timeout and retries.
    }

    puts("Process complete");
    return 0;
}