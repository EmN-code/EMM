#include <string.h>
#include <net/ipv6/addr.h>
#include <net/sock/tcp.h>
#include <net/sock/udp.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response
    if (pdu == NULL) {
        printf("Response timeout or error\n");
        return;
    }

    printf("Response code: %u\n", coap_get_code(pdu));
    printf("Response payload: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
}

int main(void) {
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.port = COAP_SERVER_PORT;

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    size_t len = coap_opts_extend(&pdu, COAP_OPTION_CONTENT_FORMAT, COAP_CT_TEXT_PLAIN);
    memcpy(coap_payload(&pdu), "Hello, COAP Cloud", 16);
    len += 16;

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) > 0) {
        printf("Request sent successfully\n");
    } else {
        printf("Failed to send request\n");
    }

    return 0;
}