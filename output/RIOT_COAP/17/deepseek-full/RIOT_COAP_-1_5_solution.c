
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <time_units.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <msg.h>
#include <mbox.h>
#include <sys/socket.h>
#include <ztimer.h>
#include <xtimer.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <mma8x5x_regs.h>
#include <net/sock/util.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    } else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    } else if (memo->state == GCOAP_MEMO_RESP) {
        puts("Response received");
    }
}

int main(void) {
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    strcpy((char *)pdu.payload, "Hello, COAP Cloud");
    len += strlen("Hello, COAP Cloud");

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid IP address");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}