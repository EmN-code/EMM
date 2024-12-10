
#include <stdio.h>
#include <net/gcoap.h>
#include <evtimer_msg.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <time_units.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <msg.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <sys/socket.h>
#include <ztimer.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Request timed out\n");
    } else if (memo->state == GCOAP_MEMO_ERR) {
        printf("Request error\n");
    } else if (memo->state == GCOAP_MEMO_EXCEPTION) {
        printf("Request exception\n");
    } else if (memo->state == GCOAP_MEMO_SUCCESS) {
        printf("Request successful\n");
    }
}

int main(void) {
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Failed to convert IP address\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    ssize_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (len < 0) {
        printf("Failed to finish CoAP options\n");
        return 1;
    }

    gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
