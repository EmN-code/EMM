#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h> // memcpy() implicitly needs this when using memcpy()
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <net/gcoap.h>

#define SERVER_PORT        5683
#define SERVER_IP          "47.102.103.1"
#define URI_PATH           "/coap/test"
#define PAYLOAD            "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote){
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Timeout, no response received.");
        return;
    }

    if (pdu->hdr->code == COAP_CODE_204) {
        puts("Success: CoAP response received");
    }
    else {
        printf("Error: CoAP response error %d\n", pdu->hdr->code);
    }
}

int main(void) {
    // Prepare the buffer and PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .port = SERVER_PORT };  

    // Set the remote IP address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Failed to convert IP address");
        return 1;
    }

    // Initialize the CoAP PDU
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    // Set the payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP request
    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Failed to send request");
        return 1;
    }

    puts("CoAP request sent");
    
    return 0;
}