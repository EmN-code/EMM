
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <xtimer.h>
#include <sys/socket.h>
#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <stdatomic.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    } else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    } else {
        puts("Request succeeded");
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Invalid IP address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_add_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
