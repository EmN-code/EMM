#include "net/sock/udp.h"
#include "net/sock.h"
#include <stdlib.h> // for exit
#include <stdio.h>  // for printf
#include <string.h> // for memset and memcpy
#include <arpa/inet.h> // for inet_pton

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    printf("Response received from server\n");
}

int main(void) {
    // Create a buffer for the CoAP request
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Initialize a UDP endpoint
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.port = COAP_SERVER_PORT;

    // Prepare the CoAP packet
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    size_t len = coap_payload_len(&pdu);

    // Send the CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
        exit(EXIT_FAILURE);
    }

    printf("CoAP request sent\n");

    return 0;
}