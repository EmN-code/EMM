#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/sock.h>
#include "kernel_defines.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define CONFIG_GCOAP_PORT 5683
#define COAP_METHOD_POST 2

// CoAP server details
#define COAP_SERVER "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"

// Message content
#define COAP_MESSAGE "Hello, COAP Cloud"

// Globally declare required variables
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

gcoap_response(coap_pkt_t *pdu, uint8_t *buf, size_t len, unsigned code);

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response if needed (print or process)
    (void)memo; // Suppress unused variable warning
    (void)pdu;
    (void)remote;
}

int main(void)
{
    sock_udp_ep_t remote_ep = { .family = AF_INET6, .port = COAP_PORT }; // Simplified placeholder for address
    inet_pton(AF_INET6, COAP_SERVER, &remote_ep.addr.ipv6);
    
    // Initialize the CoAP packet
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    // Add payload to the packet
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    int payload_len = strlen(COAP_MESSAGE);

    // Set up the length of the UDP packet
    ssize_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (len < 0) {
        // Handle error in finishing options
        return -1;
    }

    // Send CoAP request
    if (gcoap_req_send(buf, len + payload_len, &remote_ep, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        // Handle error in sending CoAP request
        return -1;
    }

    return 0;
}