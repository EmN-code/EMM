#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/ipv6/addr.h"

#define SERVER_IP            "47.102.103.1"
#define SERVER_PORT          CONFIG_GCOAP_PORT
#define URI_PATH             "/coap/test"
#define BUFFER_SIZE          128

// This is the handler that processes the response from the server.
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        printf("Response received: %d\n", coap_get_code(pdu));
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[BUFFER_SIZE];  // Buffer for the CoAP message
    ssize_t payload_len;
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    
    // Initialize CoAP packet
    payload_len = gcoap_req_init(&pdu, &buf, BUFFER_SIZE, COAP_METHOD_POST, URI_PATH);

    // Set payload
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    payload_len += 17;

    // Send the CoAP request
    gcoap_req_send(buf, payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;  // Exit the program
}