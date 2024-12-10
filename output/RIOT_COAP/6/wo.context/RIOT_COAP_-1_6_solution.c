// The following code demonstrates sending a CoAP POST request from an ESP32 to a remote CoAP server with the IP address '47.102.103.1'. The request sends the message 'Hello, COAP Cloud' to the URI path '/coap/test'.

#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static gcoap_request_memo_t req_memo;
static sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handler for responses from the server
    if (pdu->code == COAP_CODE_205) {
        printf("Received response: %s\n", payload_to_string(pdu));
    } else {
        printf("Error fetching response.\n");
    }
}

int main(void) {
    // Buffer for constructing CoAP message
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    
    // Initialize CoAP request PDU
    int len = gcoap_req_init(&pdu, &buf[0], CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Error initializing CoAP request\n");
        return 1;
    }
    // Add payload: 'Hello, COAP Cloud'
    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD));
    len += sizeof(PAYLOAD);

    // Send the request
    if (gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    puts("CoAP request sent\n");

    return 0;
}