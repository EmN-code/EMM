#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 2

static const char *EMQX_SERVER_IP = "47.102.103.1";
static const char *EMQX_URI_PATH = "/coap/test";

typedef struct {
    uint16_t len;
    char msg[CONFIG_GCOAP_PDU_BUF_SIZE];
} payload_t;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu == NULL) {
        puts("No response received");
        return;
    }
    printf("Received response from server\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    payload_t payload;

    // Populate remote data
    remote.family = AF_INET;
    remote.port = 5683;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP request
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    len += 17;

    // Sending CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) > 0) {
        puts("CoAP request sent");
    } else {
        puts("Failed to send CoAP request");
    }

    return 0;
}
