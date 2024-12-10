#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "timex.h"
#include "time_units.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mbox.h"

#define SERVER_PORT (5683)
#define EMQX_URI_PATH "/coap/test"
#define EMQX_IP_ADDRESS "47.102.103.1"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (pdu != NULL) {
        printf("Response code: %1u.%02u\n",
               coap_get_code_class(pdu),
               coap_get_code_detail(pdu));
    }
    else {
        printf("Request failed
");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, EMQX_IP_ADDRESS, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IP address");
        return 1;
    }
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    int res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending request: %d\n", res);
        return 1;
    }

    return 0;
}