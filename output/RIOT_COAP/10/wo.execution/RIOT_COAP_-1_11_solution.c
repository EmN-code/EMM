#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "sched.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/nanocoap/cache.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static const sock_udp_ep_t remote = {
    .family = AF_INET,
    .port = SERVER_PORT
};

static void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t *remote){
    (void)remote;
    if (req_state == GCOAP_MEMO_COMPLETE) {
        // Handle response
        printf("Response received\n");
    } else {
        // Handle error
        printf("Error occurred\n");
    }
}

int main(void){
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}