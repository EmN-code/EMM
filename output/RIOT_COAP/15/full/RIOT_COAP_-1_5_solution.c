#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <architecture.h>

#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define CONFIG_GCOAP_PDU_BUF_SIZE 256

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;
    int rc;

    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 18);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    while (1) {
        res = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Error sending CoAP request\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("CoAP request sent successfully\n");
            break;
        }
    }
    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;  // suppress unused variable warning
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout in response\n");
        return;
    }
    if (pdu == NULL) {
        printf("Remote did not send a response\n");
        return;
    }
    printf("Received response: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
}