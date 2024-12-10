#include <net/sock/udp.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    if (pdu) {
        printf("Response received\n");
    } else {
        printf("Error in response\n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, &buf[0], CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, COAP_MSG, strlen(COAP_MSG));
    pdu.payload_len = strlen(COAP_MSG);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    return 0;
}