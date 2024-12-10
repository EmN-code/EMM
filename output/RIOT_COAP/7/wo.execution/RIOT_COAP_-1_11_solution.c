#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "string.h"
#include "net/sock/util.h"
#include "arpa/inet.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "coap_pkt.h"
#include "kernel_defines.h"
#include "dht.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received!\n");
}

int main(void) {
    uint8_t buf[128];
    coap_pkt_t pdu;
    size_t len;

    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = COAP_SERVER_PORT
    };

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return -1;
    }

    len = coap_pkt_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    uint8_t *payload = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    strcpy((char *)payload, "Hello, COAP Cloud");
    coap_hdr_set_payload_len(&pdu, strlen("Hello, COAP Cloud"));

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: unable to send CoAP request\n");
        return -1;
    }

    printf("CoAP Request sent\n");
    
    // Keep running to handle response
    while (1) {
        xtimer_usleep(1000000);
    }

    return 0;
}