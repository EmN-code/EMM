#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "timex.h"
#include "time_units.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <unistd.h>
#include <ztimer.h>
#include <xtimer.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <mutex.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote){
    printf("Response received from %d\n", remote->port);
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Invalid server IP\n");
        return -1;
    }

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    if (len == 0){
        printf("Error: Failed to initialize request\n");
        return -1;
    }

    memcpy(coap_payload(&pdu), COAP_MESSAGE, sizeof(COAP_MESSAGE));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: Failed to send request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}