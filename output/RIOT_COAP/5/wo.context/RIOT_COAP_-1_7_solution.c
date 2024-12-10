#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/sock/dtls.h>
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static ssize_t gcoap_req_init(coap_pkt_t *pdu, uint8_t *buf, size_t len, unsigned code, const char *path) {
    // Initialize CoAP packet
    printf("Initializing CoAP packet\n");
    return 0; // This function is typically library defined, here just for illustration
}

static ssize_t gcoap_req_send(const uint8_t *buf, size_t len, const sock_udp_ep_t *remote, gcoap_resp_handler_t resp_handler, void *context, gcoap_socket_type_t tl_type) {
    // Send CoAP request
    printf("Sending CoAP request to %s:%d\n", SERVER_IP, SERVER_PORT);
    return 0; // This function is typically library defined, here just for illustration
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    uint8_t buf[128]; // CONFIG_GCOAP_PDU_BUF_SIZE

    // Network Configuration
    int result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        perror("inet_pton");
        return -1; // Exit in case of error
    }
    remote.port = SERVER_PORT;

    // Initialize CoAP packet
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH) < 0) {
        fprintf(stderr, "Error initializing CoAP request\n");
        return -1;
    }

    // Copy payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    if (gcoap_req_send(&buf[0], sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        fprintf(stderr, "Error sending CoAP request\n");
        return -1;
    }

    // Normally the program would wait and handle responses asynchronously
    // Here we just simulate an exit after send
    printf("CoAP request sent\n");
    return 0;
}