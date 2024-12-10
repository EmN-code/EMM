#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "msg.h"
#include "mutex.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "mma8x5x.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "event/thread.h"
#include "net/emcute.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;

    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("gcoap: timeout\n");
        return;
    }
    if (pdu != NULL) {
        printf("gcoap: response received - %u\n", coap_get_code(pdu));
    } else {
        printf("gcoap: no response\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t res;
    sock_udp_ep_t remote;

    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    gcoap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    while (1) {
        res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Error sending CoAP request. Retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
            continue;
        }
        printf("CoAP request sent successfully.\n");
        break;
    }

    return 0;
}