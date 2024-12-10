
#include <stdio.h>
#include <netinet/in.h>
#include "gcoap.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("gcoap: timeout");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("gcoap: error in response");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        char *buf = (char *)pdu->payload;
        printf("gcoap: response %u.%02u: %s\n",
               (unsigned)COAP_CODE_CLASS(pdu->code),
               (unsigned)COAP_CODE_DETAIL(pdu->code), buf);
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    gcoap_req_init(buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(buf, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(buf, COAP_OPT_FINISH_PAYLOAD);
    memcpy(buf + len, "Hello, COAP Cloud", 17);
    len += 17;

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        gcoap_process();
    }

    return 0;
}
