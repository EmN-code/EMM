#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

int main(void) {
    // Define buffer to hold the CoAP PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    // Define the CoAP packet structure
    coap_pkt_t pdu;
    // Define the CoAP server endpoint parameters
    sock_udp_ep_t coap_server = { .family = AF_INET6, .addr.ipv6 = { 0x00 }, .port = 5683 };
    inet_pton(AF_INET6, "47.102.103.1", coap_server.addr.ipv6);

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    // Create the payload for the CoAP POST request
    const char* payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Send the CoAP POST request to the server
    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &coap_server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Failed to send CoAP request: %d\n", (int)res);
    } else {
        printf("CoAP request sent successfully\n");
    }

    return 0;
}

// Callback function to handle server responses
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Received response from server\n");
}