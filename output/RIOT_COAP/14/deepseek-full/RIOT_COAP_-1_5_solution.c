
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <xtimer.h>
#include <dht_params.h>
#include <time_units.h>
#include <net/emcute.h>
#include <msg.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <stddef.h>
#include <sys/socket.h>
#include <ztimer.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        printf("Response Code: %02u\n", coap_get_code(pdu));
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Invalid IP address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    gcoap_req_send(buf, pdu.offset, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
