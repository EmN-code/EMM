// Required Imports
#include <stdio.h>
#include <net/nanocoap.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

// Buffer for the CoAP packet
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

// Function to handle CoAP response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if(memo->state == GCOAP_MEMO_RESP){
        printf("Response received from CoAP server\n");
    }
    else {
        printf("No response from CoAP server\n");
    }
}

int main(void) {
    // Initialize CoAP PDU
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = CONFIG_GCOAP_PORT };
    inet_pton(AF_INET6, "47.102.103.1", &remote.addr);

    // Initialize PDU
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    // Set payload for the POST request
    uint8_t payload[] = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, sizeof(payload));
    pdu.payload_len = sizeof(payload);

    // Send CoAP request
    if (gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("CoAP request failed\n");
    } else {
        printf("CoAP request sent\n");
    }

    return 0;
}