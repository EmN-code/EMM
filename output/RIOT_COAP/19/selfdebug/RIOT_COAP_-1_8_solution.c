#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/inet.h"
#include "net/sock/udp.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
sock_udp_ep_t remote;

// Function prototype for coap_hdr_len, assuming it's a utility function for header length
size_t coap_hdr_len(coap_pkt_t *pdu);

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
    // Handle response here...
}

int main(void) {
    // Setup remote server details
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP packet and buffer
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    size_t len = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len == 0) {
        printf("Error sending request\n");
        return 1;
    }

    printf("Request sent\n");
    return 0;
}