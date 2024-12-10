
#include <stdio.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <esp_wifi.h>
#include <xtimer.h>
#include <msg.h>
#include <mutex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <time_units.h>
#include <timex.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <net/emcute.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        char *resp_buf = (char *)pdu->payload;
        printf("Received response: %s\n", resp_buf);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Invalid server IP address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_add_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    gcoap_req_send(buf, coap_get_total_pkt_len(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
