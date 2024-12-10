#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE (128)
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // handle response, this could be logging the response, etc.
}

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683};

    // Convert the server IP from text to binary form
    if (inet_pton(AF_INET6, EMQX_SERVER_IP, &remote.addr.ipv6) != 1) {
        // Handle error
        return -1;
    }

    // Initialize CoAP request
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Copy payload into CoAP packet
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        // Handle error
        return -1;
    }

    return 0;
}