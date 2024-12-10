#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "dht.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "timex.h"
#include "time_units.h"
#include "xtimer.h"
#include "ztimer.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "unistd.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) memo;
    (void) remote;
    if (pdu != NULL) {
        puts("Response received");
    } else {
        puts("No response received");
    }
}

int main(void) {
    uint8_t buf[128];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    if (!inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4)) {
        puts("Error: unable to parse IPv4 address");
        return -1;
    }

    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (len < 0) {
        puts("Error: unable to initialize CoAP request");
        return -1;
    }

    coap_pkt_append_payload(&pdu, (uint8_t *)COAP_MESSAGE, strlen(COAP_MESSAGE));

    if (gcoap_req_send(buf, pdu.payload_len + len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error: unable to send CoAP request");
        return -1;
    }

    puts("CoAP request sent");
    return 0;
}