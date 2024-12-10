#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdint.h"
#include "stdatomic.h"
#include "string.h"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "mutex.h"
#include "msg.h"
#include "kernel_defines.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht.h"
#include "dht_params.h"
#include "mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "ipv6/addr.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "time_units.h"

define COAP_PORT 5683

static const char *EMQX_SERVER_IP = "47.102.103.1";  
static const char *EMQX_URI_PATH = "/coap/test";
static const char *PAYLOAD = "Hello, COAP Cloud";

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu == NULL) {
        printf("gcoap: timeout\n");
        return;
    }
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int rc;

    while (1) {
        rc = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
        if (rc <= 0) {
            printf("inet_pton failed\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("inet_pton succeeded\n");
            break;
        }
    }

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON); 
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));  
    gcoap_finish(&pdu, len, strlen(PAYLOAD));

    while (1) {
        ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res <= 0) {
            printf("gcoap_req_send failed\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Request sent successfully\n");
            break;
        }
    }
    return 0;
}