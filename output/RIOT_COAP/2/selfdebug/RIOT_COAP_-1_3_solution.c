// Include necessary header files
#include "net/ipv6/addr.h"
#include "net/gcoap.h"

// Define the CoAP server IP and port
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

// Define the payload to be sent
#define PAYLOAD "Hello, COAP Cloud"

// Define the buffer size for PDU
#define PDU_BUF_SIZE 128

// Buffer and PDU for the request
static uint8_t buf[PDU_BUF_SIZE];

int main(void) {
    // Initialize structures for endpoint and CoAP packet
    sock_udp_ep_t remote = { .port = SERVER_PORT };
    coap_pkt_t pdu;

    // Set the server address (Assuming server uses IPv4)
    ipv6_addr_from_str((ipv6_addr_t *)&remote.ipv4, SERVER_ADDR);

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Send the CoAP request
    ssize_t bytes_sent = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        puts("Error sending CoAP request");
    } else {
        printf("CoAP request sent: %d bytes\n", (int)bytes_sent);
    }

    return 0;
}