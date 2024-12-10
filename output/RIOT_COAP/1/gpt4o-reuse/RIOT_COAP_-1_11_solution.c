
#include "gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "fmt.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    if (pdu->hdr->code == COAP_CODE_CONTENT) {
        uint8_t buf[32];
        size_t len = coap_get_payload(pdu, &buf);
        if (len > 0) {
            printf("Received response: %.*s\n", (int)len, buf);
        }
    }
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    gcoap_req_init(&buf[0], GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    coap_hdr_set_type(pdu->hdr, COAP_TYPE_CON);
    coap_hdr_set_code(pdu->hdr, COAP_CODE_POST);
    coap_hdr_set_token(pdu->hdr, NULL, 0);

    const char *payload = "Hello, COAP Cloud";
    coap_add_payload(&buf[0], GCOAP_PDU_BUF_SIZE, (uint8_t *)payload, strlen(payload));

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    gcoap_req_send(&buf[0], coap_opt_finish(&buf[0], COAP_FORMAT_TEXT), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
