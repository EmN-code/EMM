#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "sys/socket.h"
#include "unistd.h"
#include "time_units.h"
#include "string.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "net/emcute.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _response_handler(const coap_pkt_t *pdu, const sock_udp_ep_t *remote, void *context)
{
    (void)context;  // Unused parameter
    puts("Received response");
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = EMQX_PORT;

    int res = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("inet_pton error");
        return 1;
    }

    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    if (coap_opt_finish(&pdu, len) < 0) {
        puts("coap_opt_finish error");
        return 1;
    }

    ssize_t gcoap_res = gcoap_req_send(buf, pdu.used, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (gcoap_res <= 0) {
        puts("gcoap_req_send error");
        return 1;
    }

    puts("Request sent");
    return 0;
}