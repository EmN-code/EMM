
#include <stdio.h>
#include <netinet/in.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("gcoap: timeout");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("gcoap: error in response");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        char *class_str = (COAP_CLASS(coap_get_code(pdu)) == COAP_CLASS_SUCCESS) ? "Success" : "Error";
        printf("gcoap: response %s, code %1u.%02u\n", class_str, COAP_CLASS(coap_get_code(pdu)), COAP_DETAIL(coap_get_code(pdu)));
        if (coap_get_payload(pdu, &payload)) {
            printf("gcoap: response payload: %s\n", (char *)payload);
        }
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    gcoap_req_init(buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(buf, COAP_FORMAT_TEXT_PLAIN);
    size_t len = coap_opt_finish(buf, COAP_OPT_FINISH_PAYLOAD);
    memcpy(buf + len, PAYLOAD, sizeof(PAYLOAD) - 1);
    len += sizeof(PAYLOAD) - 1;

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
