#include <net/gcoap.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/inet/pton.h>
#include <stdio.h>
#include <string.h>
#include <shell.h>

#define REMOTE_IP          "47.102.103.1"
#define COAP_PORT          5683
#define COAP_URI           "/coap/test"
#define COAP_PAYLOAD       "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;  // Silence unused parameter warning
    if (pdu != NULL) {
        printf("Received response: %u\n", coap_get_code(pdu));
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_PORT;
    inet_pton(AF_INET, REMOTE_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    pdu.payload_len = strlen(COAP_PAYLOAD);

    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
    }

    return 0;
}