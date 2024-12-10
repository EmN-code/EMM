#include "unistd.h"
#include "time_units.h"
#include "thread_config.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "sys/socket.h"
#include "stdbool.h"
#include "stddef.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "netinet/in.h"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;    /* not interested in the source address */
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("timeout\n");
        return;
    }
    if (memo->state != GCOAP_MEMO_RESP) {
        printf("error occurred\n");
        return;
    }

    coap_get_code(pdu) == COAP_CODE_205 ? printf("Success\n") : printf("Failed\n");
}

int main(void) {
    uint8_t buf[128];
    size_t len;
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");
    coap_opt_add_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}