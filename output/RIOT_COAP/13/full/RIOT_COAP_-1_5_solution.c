#include "xtimer.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/ipv6/addr.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "ipv6/addr.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "coap2/coap.h"  // RIOT CoAP client library

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_REQUEST_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Timeout while waiting for response");
    }
    else if (pdu->hdr->code == COAP_CODE_205) {
        puts("Response from server: Success");
    }
    else {
        printf("Response from server: %u\n", pdu->hdr->code);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int rc;

    sock_udp_ep_t remote = { .family = AF_INET6 };
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv6);

    while (1) {
        rc = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_REQUEST_PATH);
        if (rc == -1) {
            printf("Error initializing CoAP request\n");
            xtimer_sleep(1);
            continue;
        }

        size_t len = coap_body(&pdu, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

        rc = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc == -1) {
            printf("Error sending CoAP request\n");
            xtimer_sleep(1);
            continue;
        }

        puts("CoAP request sent successfully");

        /* Let's wait some time between attempts to avoid spamming the server */
        xtimer_sleep(5);
    }

    return 0;
}