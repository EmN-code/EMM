#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/gcoap.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE (128)

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    printf("Response received: %.*s\n", pdu->payload_len, (char *)pdu->payload);
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;

    // Set the remote address and port for the CoAP server
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    remote.port = 5683;

    // Initialize the CoAP request packet
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    // Set the payload for the CoAP request
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Send CoAP request
    ssize_t result = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (result <= 0) {
        printf("Error sending CoAP request\n");
    }

    return 0;
}