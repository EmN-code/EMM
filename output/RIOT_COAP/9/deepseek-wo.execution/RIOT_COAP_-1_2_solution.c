
#include <net/ipv6/addr.h>
#include <netinet/in.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <sys/socket.h>
#include <evtimer_msg.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
        size_t len = gcoap_pkt_print(pdu, buf, sizeof(buf));
        printf("Received CoAP response: %s\n", buf);
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    remote.port = SERVER_PORT;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, MESSAGE, sizeof(MESSAGE) - 1);

    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}