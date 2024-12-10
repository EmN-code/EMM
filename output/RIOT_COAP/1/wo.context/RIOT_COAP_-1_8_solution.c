#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <coap2/coap.h>

// Define CoAP server settings
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    // Buffer for constructing CoAP PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Configure server endpoint
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    
    // Convert server IP address string to network form
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) <= 0) {
        printf("Error: Invalid IP address format\n");
        return -1;
    }

    // Initialize CoAP request as POST
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH) < 0) {
        printf("Error: Failed to initialize CoAP request\n");
        return -1;
    }

    // Set payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Finalize PDU
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE) < 0) {
        printf("Error: Failed to finalize CoAP packet\n");
        return -1;
    }

    // Send the CoAP request
    if (gcoap_req_send(&buf[0], pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error: CoAP request failed to send\n");
        return -1;
    }

    printf("CoAP request sent successfully to %s\n", SERVER_ADDR);
    return 0;
}