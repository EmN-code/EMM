#include "evtimer_msg.h"
#include "dht_params.h"
#include "dht.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    (void)remote;
    printf("Response handler invoked\n");
    /* Handle response here */
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t bytes_sent;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;

    int res = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        printf("Failed to parse IP address\n");
        return -1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    bytes_sent = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote,
                                _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes_sent <= 0) {
        printf("Failed to send request\n");
    }

    return 0;
}