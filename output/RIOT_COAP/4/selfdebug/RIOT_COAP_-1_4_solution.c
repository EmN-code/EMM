#include "string.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

// Defines the buffer size for PDU
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        puts("Timeout for CoAP request");
    }
    else if (pdu != NULL) {
        printf("Received CoAP response %u bytes long
", pdu->payload_len);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;
    sock_udp_ep_t remote;

    // Set server address
    remote.family = AF_INET;
    remote.port = 5683;
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) == 0) {
        printf("Error: unable to encode the IP address\n");
        return 1; // Error
    }

    // Initialise CoAP request
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (len < 0) {
        printf("Error: unable to initialize request\n");
        return 1; // Error
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    len = gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len < 0) {
        printf("Error: unable to send request\n");
        return 1; // Error
    }

    puts("Message sent successfully");
    return 0; // Success
}