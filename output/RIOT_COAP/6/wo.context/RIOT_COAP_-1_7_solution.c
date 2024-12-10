// Program to send a CoAP POST request from ESP32 using RIOT

#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

// Define the server IP and port
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683

// Define the CoAP URI path
#define COAP_URI_PATH "/coap/test"

// Define the CoAP message payload
#define PAYLOAD "Hello, COAP Cloud"

// Function to be called upon response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;    // Unused parameter
    if (memo->state == GCOAP_MEMO_RESP) {
        if (pdu) {
            printf("Response %u\n", coap_get_code(pdu));
        } else {
            printf("Response was NULL\n");
        }
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    // Buffer setup for GCoAP
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Setup the remote socket
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Unable to parse server address\n");
        return -1;
    }

    // Initialize CoAP request
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send the CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    printf("CoAP POST request sent\n");

    return 0;
}