#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "sched.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/nanocoap/cache.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "msg.h"
#include "kernal_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define COAP_PORT 5683

static void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (req_state == GCOAP_MEMO_STATE_RESP) {
        printf("Response received\n");
    }
    else {
        printf("No response\n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t len;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = COAP_PORT;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    len += strlen(PAYLOAD);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    return 0;
}
