#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "msg.h"
#include "mutex.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"

#define REQ_MESSAGE "Hello, COAP Cloud"
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo) {
        printf("Response received.\n");
    } else {
        printf("No response received.\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t req_len;
    int rc;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;

    rc = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (rc != 1) {
        printf("Failed to convert IP address\n");
        return 1;
    }

    rc = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (rc < 0) {
        printf("Failed to initialize request\n");
        return 1;
    }

    memcpy(pdu.payload, REQ_MESSAGE, sizeof(REQ_MESSAGE));
    pdu.payload_len = sizeof(REQ_MESSAGE);

    while (1) {
        req_len = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (req_len <= 0) {
            printf("Failed to send request\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        }
        printf("Request sent successfully\n");
        break;
    }
    return 0;
}