
#include <stdlib.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <net/sock/udp.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Response handling code here
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        // Handle error
        return 1;
    }

    // Initialize CoAP request
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH) < 0) {
        // Handle error
        return 1;
    }

    // Add payload to the CoAP request
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    if (coap_pkt_set_payload(&pdu, (uint8_t *)payload, payload_len) < 0) {
        // Handle error
        return 1;
    }

    // Send the CoAP request
    if (gcoap_req_send(buf, pdu.offset, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        // Handle error
        return 1;
    }

    return 0;
}
