
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <xtimer.h>
#include <msg.h>
#include <unistd.h>
#include <stdlib.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    } else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    } else {
        printf("Received response with code %02x\n", coap_get_code(pdu));
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Invalid IP address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_hdr_set_token(pdu.hdr, (uint8_t *)"token", 5);
    coap_add_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    gcoap_req_send(buf, coap_get_total_bytes(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
