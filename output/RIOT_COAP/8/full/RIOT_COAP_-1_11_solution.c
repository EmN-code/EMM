#include "ztimer.h"
#include "xtimer.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "msg.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "arpa/inet.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        puts("timeout received");
    }
    else {
        puts("success: response received");
    }
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    int res;

    while (1) {
        res = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
        if (res != 1) {
            printf("Failed to convert IP address\n");
            xtimer_sleep(1);
            continue;
        }

        gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
        memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
        gcoap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

        res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res <= 0) {
            printf("Failed to send request: %d\n", res);
            xtimer_sleep(1);
        } else {
            printf("Request sent successfully\n");
            break;
        }
    }
    while(1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep to keep the process alive
    }

    return 0;
}