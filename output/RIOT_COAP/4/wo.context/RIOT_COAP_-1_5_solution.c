#include <net/sock/udp.h>
#include <net/coap.h>
#include <stdio.h>

// Define constants for CoAP configuration
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

// Define message payload
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    // Network and CoAP PDU buffers
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Convert IP address for remote CoAP server
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Could not convert IP address\n");
        return -1;
    }
    remote.port = COAP_SERVER_PORT;

    // Initialize coap packet
    ssize_t result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (result < 0) {
        printf("Error: CoAP request init failed\n");
        return -1;
    }

    // Copy the payload into the packet
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP request
    if (gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error: CoAP request send failed\n");
        return -1;
    }

    printf("CoAP request sent successfully\n");

    return 0;
}