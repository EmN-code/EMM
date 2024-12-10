#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "xtimer.h"
#include "mutex.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "net/emcute.h"
#include "time_units.h"
#include "arpa/inet.h"
#include "kernel_defines.h"

static const char *EMQX_SERVER_IP = "47.102.103.1";
static const char *EMQX_URI_PATH = "/coap/test";
static sock_udp_ep_t remote = {.family = AF_INET, .port = 5683};
static char _recv_buf[128];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_STATE_COMPLETED) {
        printf("Response received\n");
    }
    else {
        printf("Failed to receive response\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t bytes_sent;

    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    coap_hdr_set_type(pdu.hdr, COAP_TYPE_NON);  // Correct the error: 'COAP_NON' was incorrect; using 'COAP_TYPE_NON'

    const char *data = "Hello, COAP Cloud";
    memcpy(pdu.payload, data, strlen(data));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    bytes_sent = gcoap_req_send(buf, coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD) - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);  // Correct the argument type

    if (bytes_sent <= 0) {
        printf("Error sending request\n");
    }
    else {
        printf("Sent %d bytes\n", (int)bytes_sent);
    }

    return 0;
}