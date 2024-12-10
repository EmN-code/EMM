#include <ztimer.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <sched.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/nanocoap/cache.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>

#define COAP_METHOD_POST 2

#ifndef CONFIG_GCOAP_PDU_BUF_SIZE
#define CONFIG_GCOAP_PDU_BUF_SIZE 256
#endif

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;

    if (req_state == GCOAP_MEMO_PDU_MISMATCH) {
        printf("[GCOAP ]Got mismatched response!\n");
        return;
    }

    printf("[GCOAP] Response received, code: %d\n", coap_get_code(pdu));
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    ssize_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);

    len += coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}