
#include <stdio.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <msg.h>
#include <ztimer.h>
#include <xtimer.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <esp_wifi.h>
#include <net/emcute.h>
#include <dht_params.h>
#include <dht.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <stdatomic.h>
#include <time_units.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
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

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid server IP address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    coap_pkt_set_payload(&pdu, (uint8_t *)payload, payload_len);

    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE) < 0) {
        puts("Failed to finalize CoAP packet");
        return 1;
    }

    gcoap_req_send(buf, pdu.offset, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
