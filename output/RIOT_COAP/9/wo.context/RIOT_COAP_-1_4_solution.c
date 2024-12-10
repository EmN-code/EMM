// This program sends a CoAP POST request to a CoAP server using RIOT on an ESP32.

#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/sock.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

// Forward Declarations
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    // Buffer to store the CoAP message to be sent
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    int result;

    // Initialize the CoAP packet with POST method
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Set the payload of the CoAP message
    const char *PAYLOAD = "Hello, COAP Cloud";
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Convert IP address from string to byte representation
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        puts("Error: inet_pton failed");
        return -1;
    }

    // Send the CoAP request
    if (gcoap_req_send(&buf[0], pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error: gcoap_req_send failed");
        return -1;
    }

    puts("CoAP POST request sent successfully");
    return 0;
}

// Response handler for the CoAP request
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP) {
        puts("Response received");
    }
    else {
        puts("No response received");
    }
}