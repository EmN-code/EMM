
#include <stdio.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <mutex.h>
#include <mma8x5x.h>
#include <xtimer.h>
#include <dht.h>
#include <dht_params.h>
#include <stdatomic.h>
#include <time_units.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <esp_wifi.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <net/emcute.h>
#include <evtimer_msg.h>
#include <mma8x5x_regs.h>
#include <stdbool.h>
#include <stddef.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        printf("Received response with code %02x\n", coap_get_code(pdu));
    }
    else {
        puts("Request failed");
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Invalid IP address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_add_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    ssize_t len = coap_get_total_pkt_len(&pdu);
    if (len <= 0) {
        puts("Failed to create CoAP request");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
