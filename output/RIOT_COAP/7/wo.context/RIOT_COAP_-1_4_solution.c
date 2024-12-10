#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <gcoap.h>

#define PAYLOAD "Hello, COAP Cloud"
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received from the server!\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = 5683;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}