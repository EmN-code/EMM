#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define REMOTE_ADDR "47.102.103.1"
#define REMOTE_PORT 5683

#define EMQX_URI_PATH "/coap/test"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128  // Make sure this is defined only once

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = REMOTE_PORT };

    // Initialize destination address
    inet_pton(AF_INET, REMOTE_ADDR, &remote.addr.ipv4);

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    // Add payload
    const char *message = "Hello, COAP Cloud";
    memcpy(pdu.payload, message, strlen(message));
    size_t payload_len = strlen(message);
    pdu.payload_len = payload_len;

    // Send CoAP request
    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending request\n");
    }

    return 0;
}