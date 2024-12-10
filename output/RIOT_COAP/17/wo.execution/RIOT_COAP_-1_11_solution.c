#include "timex.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "ztimer.h"
#include "kernel_defines.h"
#include "argparse.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht.h"
#include "dht_params.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define COAP_METHOD COAP_METHOD_POST
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

void *thread_handler(void *arg){
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) return;
    gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
}

int main(void){
    thread_handler(NULL);
    return 0;
}