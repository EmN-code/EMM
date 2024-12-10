#include <string.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <xtimer.h>

#define SERVER_IPV6_ADDR  "47.102.103.1"
#define SERVER_PORT       5683
#define URI_PATH          "/coap/test"
#define PAYLOAD           "Hello, COAP Cloud"

int main(void) {
    // Buffer for CoAP PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Initialize CoAP PDU
    coap_pkt_t pdu;
    memset(&pdu, 0, sizeof(pdu));
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Properly initialize the remote endpoint
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IPV6_ADDR);

    // Prepare for sending
    size_t payload_length = strlen(PAYLOAD);
    len += payload_length;

    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res < 0) {
        printf("Error sending CoAP request: %ld\n", res);
    } else {
        printf("CoAP request sent\n");
    }

    return 0;
}

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP_RECEIVED) {
        printf("Response received\n");
    } else {
        printf("No response received\n");
    }
}