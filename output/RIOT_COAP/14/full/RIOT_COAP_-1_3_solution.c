#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/ipv4/addr.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "arpa/inet.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT (5683)
#define COAP_URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"
#define MAX_RETRIES 5

gcoap_resp_handler_t _resp_handler;

static int _response_handler(unsigned req_state, coap_pkt_t* pdu, sock_udp_ep_t* remote) {
    if (req_state == GCOAP_MEMO_COMPLETE) {
        printf("Response received: %u\n", coap_get_id(pdu));
        return 0;
    }
    printf("Error in request state\n");
    return -1;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = {.family = AF_INET, .port = SERVER_PORT};
    coap_pkt_t pdu;

    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    int retry_count = 0;
    int res;

    while (retry_count < MAX_RETRIES) {
        gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
        size_t len = coap_pkt_build(&pdu, (uint8_t*)MESSAGE, strlen(MESSAGE), 0);

        res = gcoap_req_send(buf, len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        if (res >= 0) {
            printf("Request sent successfully\n");
            break;
        }
        else {
            printf("Failed to send request, retrying...\n");
            xtimer_sleep(1);
            retry_count++;
        }
    }

    if (res < 0) {
        printf("Exceeded maximum retries. Exiting...\n");
        return -1;
    }

    return 0;
}