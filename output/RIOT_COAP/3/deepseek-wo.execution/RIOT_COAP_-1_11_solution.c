
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>
#include <msg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <net/sock/util.h>
#include <timex.h>
#include <time_units.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <esp_wifi.h>
#include <ztimer.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

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
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    remote.port = EMQX_SERVER_PORT;
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Invalid IP address\n");
        return -1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT_PLAIN);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    coap_put_payload(&pdu, COAP_PAYLOAD, sizeof(COAP_PAYLOAD) - 1);

    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
