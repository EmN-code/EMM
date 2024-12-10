#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "system.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "dht_params.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    } else {
        printf("Error receiving response\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote;
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD));
    len += sizeof(PAYLOAD);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}