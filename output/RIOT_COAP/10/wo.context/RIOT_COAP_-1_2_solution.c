#include "net/gcoap.h"  // Include the gcoap library for CoAP functions
#include "net/ipv6/addr.h"  // Include the IPV6 address library
#include "msg.h"            // Include the messaging library
#include "stdio.h"          // Include standard input/output library
#include "stdlib.h"         // Include standard library for utility functions

#define _IPV4_ADDRESS "47.102.103.1" // Define the server IP address
#define _IPV4_PORT 5683              // Define the server port number
#define URI_PATH "/coap/test"        // Define the URI path

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE]; // Buffer for the PDU
    coap_pkt_t pdu;

    // Convert the IP address from string format to byte format
    sock_udp_ep_t remote;
    inet_pton(AF_INET, _IPV4_ADDRESS, &remote.addr.ipv4);
    remote.port = _IPV4_PORT;

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Add payload "Hello, COAP Cloud"
    size_t len = snprintf(pdu.payload, CONFIG_GCOAP_PDU_BUF_SIZE, "Hello, COAP Cloud");

    // Send CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("CoAP message sent!\n");

    // Initialize messaging (needed for RIOT applications using msg_t type)
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    // Keep the application running to receive response
    while(1) {
        // Enter sleep mode until interrupt occurs (simulating idle state)
        msg_receive(NULL);
    }

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("gcoap: timeout""");
        return;
    }
    if (memo->state == GCOAP_MEMO_RESPONSE) {
        printf("gcoap: response received, code %1u\n", coap_get_code(pdu));
    }
}
