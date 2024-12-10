#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define COAP_PORT 5683
#define PAYLOAD "Hello, COAP Cloud"

gcoap_listener_t gcoap_listener;
sock_udp_ep_t remote;
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response from server here
    printf("Response received\n");
}

int main(void) {
    // Initialize remote end point
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_PORT;

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse server address");
        return -1;
    }

    // Initialize CoAP packet
    coap_pkt_t pdu;
    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    // Set payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error: unable to send CoAP request");
        return -1;
    }

    puts("CoAP request sent.");
    return 0;
}