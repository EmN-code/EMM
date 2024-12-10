
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "mma8x5x.h"
#include "time_units.h"
#include "esp_system.h"
#include "xtimer.h"
#include "mutex.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "stdio.h"
#include "ztimer.h"
#include "stdatomic.h"
#include "dht.h"
#include "esp_wifi.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "net/sock/util.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stddef.h"
#include "mbox.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    remote.port = HTONS(EMQX_SERVER_PORT);
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_pkt_set_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17);

    gcoap_req_send(buf, coap_pkt_get_total_size(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
