#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"

#include <stdio.h>
#include <string.h>

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (req_state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
        printf("Payload: %s\n", (char *)pdu->payload);
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t server = {
        .family = AF_INET6,
        .port = COAP_SERVER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr, COAP_SERVER_ADDR);
    
    ssize_t res;
    while (1) {
        int init_res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
        if (init_res < 0) {
            printf("gcoap_req_init() failed\n");
            xtimer_sleep(1);
            continue;
        }
        memcpy(pdu.payload, COAP_MSG, strlen(COAP_MSG));
        pdu.payload_len = strlen(COAP_MSG);

        res = gcoap_req_send(buf, sizeof(buf), &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("gcoap_req_send() failed\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Request sent: %s\n", COAP_MSG);
            break;
        }
    }
    return 0;
}