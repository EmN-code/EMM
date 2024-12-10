
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <time_units.h>
#include <stdbool.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <stdatomic.h>
#include <stddef.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define BUF_SIZE 128

static void _resp_handler(const void *memo, uint8_t *pdu, const sock_udp_ep_t *remote) {
    // Response handling logic here
    printf("Response received from server\n");
}

int main(void) {
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = HTONS(EMQX_SERVER_PORT);

    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Add payload
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    memcpy(&buf[pdu.payload_len], payload, payload_len);
    pdu.payload_len += payload_len;

    // Finish CoAP options
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Convert IP address
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid address\n");
        return 1;
    }

    // Send CoAP request
    ssize_t len = coap_build(&pdu, buf, BUF_SIZE);
    if (len <= 0) {
        printf("Failed to build CoAP packet\n");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
