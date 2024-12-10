#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "net/gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static gcoap_resp_handler_t _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t* remote) {
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("Request timed out\n");
        return 0;
    }
    if (pdu) {
        printf("Response received\n");
    }
    return 0;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    int result;
    while (1) {
        result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
        if (result <= 0) {
            printf("Failed to parse IP address\n");
            xtimer_sleep(1);
            continue;
        } else {
            break;
        }
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (coap_opt_add_format(&pdu, COAP_FORMAT_TEXT) == NULL) {
        printf("Failed to add CoAP options\n");
        return -1;
    }

    if (coap_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17) == NULL) {
        printf("Failed to set payload\n");
        return -1;
    }

    ssize_t bytes_sent = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}