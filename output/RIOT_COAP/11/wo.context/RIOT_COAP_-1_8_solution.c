#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/gcoap.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128  // Ensure there's only one definition

static const char EMQX_SERVER_IP[] = "47.102.103.1";
static const char EMQX_URI_PATH[] = "/coap/test";

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle responses (if needed)
}

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;

    // Fill remote address (inet_pton included from net/ipv4/addr.h)
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = CONFIG_GCOAP_PORT;  // CoAP default port is 5683
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Failed to encode IP address\n");
        return -1;
    }

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    const char *message = "Hello, COAP Cloud";
    size_t message_len = strlen(message);
    memcpy(pdu.payload, message, message_len);
    pdu.payload_len = message_len;

    // Send request
    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP POST request sent\n");
    return 0;
}