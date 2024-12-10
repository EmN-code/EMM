#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "evtimer_mbox.h"
#include "stdatomic.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "unistd.h"
#include "kernel_defines.h"
#include "stdlib.h"
#include "netinet/in.h"
#include "stdbool.h"
#include "msg.h"
#include "stddef.h"
#include "arpa/inet.h"
#include "timex.h"
#include "esp_system.h"
#include "net/gcoap.h"
#include "sys/socket.h"
#include "evtimer_msg.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mutex.h"
#include "net/sock/udp.h"
#include "net/emcute.h"

#define GCOAP_PDU_BUF_SIZE   (256U)
#define COAP_SERVER_IP       "47.102.103.1"
#define COAP_SERVER_PORT     (5683)
#define COAP_URI_PATH        "/coap/test"
#define COAP_REQUEST_CONTENT "Hello, COAP Cloud"

static ssize_t _resp_handler(const gcoap_packet_t *pdu, unsigned _flags,
                             void *_ctx) {
    (void)pdu;
    (void)_flags;
    (void)_ctx;
    puts("Response received");
    return 0;
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    size_t len = GCOAP_PDU_BUF_SIZE;
    coap_pkt_t pdu;

    while (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) < 1) {
        puts("Invalid COAP server address");
        xtimer_sleep(1);
    }

    gcoap_req_init(&pdu, buf, len, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_REQUEST_CONTENT, sizeof(COAP_REQUEST_CONTENT) - 1);
    coap_hdr_set_payload_len(&pdu, sizeof(COAP_REQUEST_CONTENT) - 1);

    while (1) {
        ssize_t res = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res >= 0) {
            puts("CoAP request sent successfully");
            break;
        } else {
            puts("Failed to send CoAP request, retrying...");
            xtimer_sleep(1);
        }
    }

    return 0;
}