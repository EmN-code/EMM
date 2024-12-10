#include <net/nanocoap.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP      "47.102.103.1"
#define SERVER_PORT    5683
#define URI_PATH       "/coap/test"
#define MESSAGE        "Hello, COAP Cloud"

// Buffer for the CoAP request
static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

int main(void) {
    printf("Starting CoAP client example...\n");

    // IPv4 address setup
    sock_udp_ep_t remote = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF };
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP packet
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    // Set payload
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    pdu.payload_len = strlen(MESSAGE);

    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}