#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>

#define GCOAP_PAYLOAD "Hello, COAP Cloud"
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, 
                          coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu->code == COAP_CODE_CREATED) {
        printf("Success: Created\n");
    } else {
        printf("Error: Request failed\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t bytes_sent;

    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_pkt_t opts_pdu = pdu;
    size_t offset = (coap_opts_extend(&opts_pdu, COAP_OPTION_CONTENT_FORMAT, COAP_CT_TEXT_PLAIN) == 0) ? opts_pdu.hdr_len : 0;

    memcpy(&buf[offset], GCOAP_PAYLOAD, sizeof(GCOAP_PAYLOAD));
    size_t len = offset + sizeof(GCOAP_PAYLOAD);

    bytes_sent = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Error: Failed to send request\n");
    }

    return 0;
}