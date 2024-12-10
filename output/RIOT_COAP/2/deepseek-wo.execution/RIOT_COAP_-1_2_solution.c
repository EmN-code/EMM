
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <timex.h>
#include <net/gcoap.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        printf("Received CoAP response with code %02x\n", pdu->hdr->code);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    if (sock_udp_str2ep(SERVER_IP, &remote) < 0) {
        puts("Error parsing server IP address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_add_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    gcoap_req_send(buf, pdu.offset, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
