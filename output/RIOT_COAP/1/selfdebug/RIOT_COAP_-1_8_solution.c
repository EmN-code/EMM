#include <xtimer.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"

#define COAP_SERVER "47.102.103.1"
#define COAP_PORT 5683
#define COAP_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"

static gcoap_request_memo_t _request_memo;
static uint8_t _buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote)
{
    (void)remote;
    if (pdu != NULL) {
        printf("Response OK\n");
    }
    else {
        printf("No response\n");
    }
}

int main(void)
{
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;
    inet_pton(AF_INET, COAP_SERVER, &remote.addr.ipv4);

    gcoap_req_init(&pdu, _buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_PATH);
    memcpy(pdu.payload, COAP_MSG, strlen(COAP_MSG));
    pdu.payload_len = strlen(COAP_MSG);
    ssize_t res = gcoap_req_send(_buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("gcoap_req_send() failed\n");
        return 0;
    }

    xtimer_sleep(5);
    return 0;
}