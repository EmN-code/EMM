#include <string.h>            // Required for string operations
#include <net/ipv6/addr.h>    // For CoAP server IPv6 address conversion
#include <net/gcoap.h>        // Provides CoAP definitions and functions
#include <net/sock/udp.h>     // Provides UDP socket definitions

#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the CoAP response
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}