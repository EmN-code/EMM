#include <string.h>
#include <net/sock/udp.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET,
                             .port = COAP_SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: Invalid IP address.");
        return -1;
    }

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res < 0) {
        printf("Error sending CoAP request: %d\n", res);
        return -1;
    }

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu == NULL) {
        printf("No response received.\n");
        return;
    }
    printf("Response received: %.*s\n", pdu->payload_len, (char *)pdu->payload);
}