// Include required header files
#include <stdio.h>
#include <string.h>
#include <net/gcoap.h>

// Function to handle the response from the server
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("[error] Timeout");
        return;
    }
    printf("[success] Got response: %s\n", (char *)pdu->payload);
}

// Main function
int main(void) {
    uint8_t buf[128]; // Buffer for the CoAP PDU
    coap_pkt_t pdu;   // CoAP packet descriptor

    // Remote end point
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    remote.port = 5683;

    // Initialize CoAP PDU for a POST request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    pdu.payload_len = strlen("Hello, COAP Cloud");

    // Send the CoAP request
    gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    puts("CoAP request sent");

    return 0;
}