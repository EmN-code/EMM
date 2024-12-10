#include <net/coap.h>  // Includes CoAP specific functions.
#include <net/af.h>  // Required for address family definitions.
#include <kernel_types.h>  // Includes RIOT system types.
#include <msg.h>  // For message handling in RIOT.
#include <net/sock/udp.h>  // Provides structures and functions for UDP sockets.
#include <string.h>  // Standard library for string operations.

#define SERVER_IP         "47.102.103.1"
#define SERVER_PORT       (5683)
#define URI_PATH          "/coap/test"
#define BUFFER_SIZE       (CONFIG_GCOAP_PDU_BUF_SIZE)  // Assuming this is predefined.
#define PAYLOAD           "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Response handling code here
}

int main(void) {
    uint8_t buf[BUFFER_SIZE];
    memset(buf, 0, BUFFER_SIZE);

    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    int result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return -1;
    }

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, BUFFER_SIZE, COAP_METHOD_POST, URI_PATH);

    if (coap_pkt_set_payload(&pdu, PAYLOAD, strlen(PAYLOAD)) < 0) {
        puts("Error setting CoAP payload");
        return -1;
    }

    ssize_t bytes_sent = gcoap_req_send(buf, pdu.hdr.len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        puts("Error sending CoAP request");
        return -1;
    }

    msg_init_queue(msg_q, ARRAY_SIZE(msg_q));
    // Further functionality if necessary.

    return 0;
}