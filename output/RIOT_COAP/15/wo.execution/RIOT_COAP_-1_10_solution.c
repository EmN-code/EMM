#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <msg.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <dht_params.h>
#include <dht.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("GCOAP request timeout");
    }
    else if (memo->state == GCOAP_MEMO_ERR_MSG) {
        puts("GCOAP error");
    }
    else if (memo->state == GCOAP_MEMO_SENT) {
        puts("GCOAP request sent");
    }
    else if (memo->state == GCOAP_MEMO_COMPLETE) {
        puts("GCOAP complete");
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    size_t len = gcoap_finish(&pdu, strlen(PAYLOAD), COAP_FORMAT_TEXT);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending request");
    }
    else {
        puts("Request sent successfully");
    }

    return 0;
}