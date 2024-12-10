
#include <string.h>
#include <net/sock/udp.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const void *buf, size_t len, const sock_udp_ep_t *remote) {
    (void)buf;
    (void)len;
    (void)remote;
    // Handle response here if needed
}

int main(void) {
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = HTONS(EMQX_SERVER_PORT);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Convert IP address string to byte representation
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    // Send the CoAP request
    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
