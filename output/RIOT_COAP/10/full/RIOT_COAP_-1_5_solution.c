#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <xtimer.h>
#include <ztimer.h>
#include <time_units.h>
#include <sys/socket.h>
#include <unistd.h>
#include <kernel_defines.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <net/emcute.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>

#define COAP_SERVER_IP    "47.102.103.1"
#define COAP_SERVER_PORT  5683
#define COAP_URI_PATH     "/coap/test"
#define COAP_PAYLOAD      "Hello, COAP Cloud"
#define MAX_RETRY         5

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (pdu != NULL) {
        printf("Response received: %u \n", coap_get_code(pdu));
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Unable to parse IP address\n");
        return -1;
    }

    int retries = 0;
    while (retries < MAX_RETRY) {
        retries++;

        gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);

        if (!coap_opt_add_payload_marker(&pdu)) {
            printf("Error: unable to add payload marker\n");
            return -1;
        }

        memcpy(coap_payload(&pdu), COAP_PAYLOAD, sizeof(COAP_PAYLOAD) - 1);
        gcoap_req_send(buf, pdu.payload_len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        xtimer_sleep(1);
    }

    return 0;
}
