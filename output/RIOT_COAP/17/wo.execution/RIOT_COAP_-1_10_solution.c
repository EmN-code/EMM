#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "ztimer.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "stdio.h"
#include "msg.h"
#include "mutex.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"

#define SERVER_IP "47.102.103.1"
#define COAP_PORT (5683)
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    puts("Response received");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;

    int res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("Error: unable to parse IP address");
        return 1;
    }

    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        puts("Error: unable to initialize request");
        return 1;
    }
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error: unable to send request");
        return 1;
    }

    puts("Request sent");
    return 0;
}