# The code sets up a simple CoAP client on an ESP32 to send a CoAP POST request to a specified server.
# It sends the text "Hello, COAP Cloud" to a URI path on the server.

#include <sys/socket.h>          // For sockets
#include <stdio.h>               // For printf
#include <stdlib.h>              // For exit
#include <thread_config.h>       // RIOT Thread config header
#include "net/gcoap.h"         // For CoAP protocol support

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void client_resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received: %s\n", (char *)pdu->payload);
    } else {
        printf("Request failed\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Set server IP address
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP PDU
    int res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (res < 0) {
        printf("Failed to initialize CoAP request\n");
        return -1;
    }

    // Add payload
    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD) - 1);
    pdu.payload_len = sizeof(PAYLOAD) - 1;

    // Send CoAP request
    ssize_t bytes_sent = gcoap_req_send(buf, pdu.payload_len, &remote, client_resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Failed to send request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}