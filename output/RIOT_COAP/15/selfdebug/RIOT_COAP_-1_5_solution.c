#include <string.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

static sock_udp_ep_t remote;
static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

int main(void) {
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_NONCON)
    
    const char *EMQX_SERVER_IP = "47.102.103.1";
    const char *payload = "Hello, COAP Cloud";
    
    remote.port = 5683;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    memcpy(pdu.payload, payload, strlen(payload));
    gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const struct sock_udp_ep_t *remote) {
    // Handle response
}