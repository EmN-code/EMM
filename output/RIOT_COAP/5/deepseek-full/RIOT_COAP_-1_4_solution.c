
#include <stdlib.h>
#include <net/emcute.h>
#include <evtimer_msg.h>
#include <mma8x5x_regs.h>
#include <stdatomic.h>
#include <time_units.h>
#include <net/sock/udp.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <esp_wifi.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <net/sock/util.h>
#include <mutex.h>
#include <mma8x5x.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_aton(SERVER_IP, &remote.addr.ipv4);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_uri_path(&pdu, URI_PATH);
    coap_pkt_set_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    ssize_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (len <= 0) {
        printf("Failed to create CoAP request\n");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
