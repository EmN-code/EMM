#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "system/xtimer.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "msg.h"

#define COAP_SERVER_ADDR      "47.102.103.1"
#define COAP_SERVER_PORT      5683
#define COAP_URI_PATH         "/coap/test"
#define COAP_PAYLOAD          "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote)
{
    (void)remote;    /* unused */
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("CoAP request timeout");
        return;
    }
    printf("Response %u.%u.%u \n",
           coap_get_code_class(pdu),
           coap_get_code_detail(pdu),
           coap_get_code_subdetail(pdu));
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    if (inet_pton(AF_INET, COAP_SERVER_ADDR, &remote.addr.ipv4) != 1) {
        puts("Error: unable to encode target address");
        return -1;
    }

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST,
                                COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Sleep long enough for the transaction to complete
    xtimer_sleep(5);

    return 0;
}