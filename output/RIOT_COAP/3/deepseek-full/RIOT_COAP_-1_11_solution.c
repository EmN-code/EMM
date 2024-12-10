
#include <stdio.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <stdbool.h>
#include <stddef.h>
#include <mutex.h>
#include <xtimer.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <ztimer.h>
#include <msg.h>
#include <kernel_defines.h>
#include <stdatomic.h>
#include <time_units.h>
#include <net/emcute.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <esp_wifi.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD_STR "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    } else {
        char *resp_buf = (char *)pdu->payload;
        printf("Received response: %s\n", resp_buf);
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
        puts("Invalid server IP address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_add_payload(&pdu, PAYLOAD_STR, strlen(PAYLOAD_STR));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    ssize_t len = coap_get_total_pkt_len(&pdu);
    if (len <= 0) {
        puts("Failed to create CoAP request");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
