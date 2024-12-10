#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int result;
    ssize_t bytes_sent;

    // Initialize remote endpoint
    memset(&remote, 0, sizeof(sock_udp_ep_t));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    // Convert IP address from string to binary
    result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        fprintf(stderr, "Error: invalid IP address format\n");
        return -1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    bytes_sent = gcoap_req_send(buf, sizeof(buf), &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        fprintf(stderr, "Error sending CoAP request: %zd\n", bytes_sent);
        return -1;
    }

    printf("CoAP request sent successfully. Bytes sent: %zd\n", bytes_sent);
    return 0;
}