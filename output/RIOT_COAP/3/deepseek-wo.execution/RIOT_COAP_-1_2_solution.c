
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

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    if (sock_udp_name2ep(&remote, SERVER_IP, NULL) < 0) {
        printf("Error parsing server address\n");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);

    // Manually add payload to the CoAP packet
    pdu.payload = (uint8_t *)payload;
    pdu.payload_len = payload_len;

    ssize_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    if (len <= 0) {
        printf("Failed to finalize CoAP packet\n");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
