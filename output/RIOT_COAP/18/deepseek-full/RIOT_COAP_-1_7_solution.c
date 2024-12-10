
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

typedef struct {
    uint8_t addr[16];
    uint16_t port;
    uint8_t family;
} sock_udp_ep_t;

typedef struct {
    uint8_t *payload;
    size_t payload_len;
} coap_pkt_t;

typedef void (*gcoap_resp_handler_t)(const void *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

static void _resp_handler(const void *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    uint8_t buf[128];
    size_t len;
    int result;

    // Initialize remote endpoint
    remote.family = AF_INET;
    remote.port = htons(EMQX_SERVER_PORT);
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr);
    if (result != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }

    // Initialize CoAP packet
    pdu.payload = buf;
    pdu.payload_len = sizeof(buf);
    len = snprintf((char *)pdu.payload, pdu.payload_len, "%s", PAYLOAD);

    // Send CoAP request
    gcoap_req_send(pdu.payload, len, &remote, _resp_handler, NULL, 0);

    return 0;
}

ssize_t gcoap_req_send(const uint8_t *buf, size_t len,
                       const sock_udp_ep_t *remote,
                       gcoap_resp_handler_t resp_handler, void *context,
                       int tl_type) {
    // Simulate sending the CoAP request
    printf("Sending CoAP request to %s:%d\n", EMQX_SERVER_IP, EMQX_SERVER_PORT);
    resp_handler(context, NULL, remote);
    return len;
}
