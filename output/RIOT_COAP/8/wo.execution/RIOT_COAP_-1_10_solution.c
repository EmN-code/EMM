#include "arpa/inet.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "dht.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "msg.h"
#include "mbox.h"
#include "stdlib.h"
#include "kernel_defines.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "unistd.h"
#include "time_units.h"
#include "dht_params.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "esp_wifi.h"
#include "mma8x5x.h"
#include "net/emcute.h"
#include "esp_system.h"
#include "stdbool.h"
#include "mma8x5x_params.h"
#include "stdio.h"
#include "ztimer.h"
#include "netinet/in.h"
#include "evtimer_mbox.h"
#include "stddef.h"
#include "net/sock/udp.h"

#define PAYLOAD                 "Hello, COAP Cloud"
#define EMQX_SERVER_IP          "47.102.103.1"
#define CONFIG_GCOAP_PDU_BUF_SIZE 256

static sock_udp_ep_t remote;
static const char *EMQX_URI_PATH = "/coap/test";

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;    // Silence unused parameter warning
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("timeout (not reachable?)\n");
        return;
    } else if (pdu == NULL) {
        printf("error in response\n");
        return;
    }
    printf("Success: received response\n");
}

int main(void) {
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.port = 5683;
    
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);
    
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
    }

    return 0;
}