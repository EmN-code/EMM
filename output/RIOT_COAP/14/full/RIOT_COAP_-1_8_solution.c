#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "timex.h"
#include "kernel_defines.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "dht.h"
#include "dht_params.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "unistd.h"

// Placeholder URI path and IP
#define COAP_URI_PATH "/coap/test"
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683

void coap_send_request(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .port = COAP_SERVER_PORT };

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("IP address parsing failed\n");
        return;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 18);
    coap_hdr_set_code(&pdu, COAP_METHOD_POST);
    pdu.payload_len = 18;

    ssize_t res;
    while (1) {
        res = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Failed to send CoAP request, retrying in 1s\n");
            xtimer_sleep(1);
        } else {
            printf("CoAP request sent\n");
            break;
        }
    }
}

int main(void) {
    coap_send_request();
    return 0;
}