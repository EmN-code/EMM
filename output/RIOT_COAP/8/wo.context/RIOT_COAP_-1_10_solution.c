#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    ssize_t result;

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Configure the remote endpoint
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error: unable to format destination address");
        return 1;
    }
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    // Send CoAP request
    result = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        puts("Error: failed to send CoAP request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}

// Response handler function
to handle responses from the server
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (pdu) {
        printf("Response received: %d\n", coap_get_code(pdu));
    } else {
        printf("Request timeout\n");
    }
}