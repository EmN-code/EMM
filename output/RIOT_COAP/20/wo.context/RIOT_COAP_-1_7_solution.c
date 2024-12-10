#include <string.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // handle response
}

int main(void) {
    // Setup the CoAP endpoint
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    const char* EMQX_SERVER_IP = "47.102.103.1";
    const char* EMQX_URI_PATH = "/coap/test";
    const char* PAYLOAD = "Hello, COAP Cloud";
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Convert IP string to a binary format
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        // handle error
    }

    // Initialize CoAP PDU
    coap_pkt_t pdu;
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}