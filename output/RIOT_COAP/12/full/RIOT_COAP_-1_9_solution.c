#include "ztimer.h"
#include "xtimer.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht.h"
#include "kernel_defines.h"
#include "arpa/inet.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"
#define COAP_MSG_LEN 18

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout from %s
", COAP_SERVER_IP);
    }
    else {
        printf("Response status: %u
", coap_get_code(pdu));
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    int result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (result == 0) {
        printf("Invalid server address
");
        return -1;
    }
    else if (result < 0) {
        printf("AF_INET not supported
");
        return -1;
    }

    remote.port = COAP_SERVER_PORT;

    while (1) {
        ssize_t rc = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
        if (rc == 0) {
            printf("Error initializing request
");
            continue;
        }

        coap_hdr_set_type(&pdu, COAP_TYPE_CON);
        coap_pkt_append_payload_marker(&pdu);
        if (!coap_pkt_append_payload(&pdu, (uint8_t *)COAP_MSG, COAP_MSG_LEN)) {
            printf("Error appending payload
");
            continue;
        }

        rc = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc <= 0) {
            printf("Error sending message
");
        }
        else {
            printf("Message sent
");
        }

        xtimer_sleep(10);
    }
    return 0;
}