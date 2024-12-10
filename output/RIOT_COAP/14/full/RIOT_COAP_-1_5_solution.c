#include <stdbool.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <net/emcute.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <kernel_defines.h>
#include <timex.h>
#include <time_units.h>

#define SENSOR_DATA "Hello, COAP Cloud"
#define COAP_SERVER "47.102.103.1"
#define COAP_PORT (5683)
#define COAP_URI_PATH "/coap/test"

void *_resp_handler(coap_pkt_t *pkt, coap_reply_t *reply, coap_retransmit_t *retr, coap_sock_t *sock, void *arg) {
    (void)reply;
    (void)retr;
    (void)sock;
    (void)arg;
    printf("Response received\n");
    return NULL;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    coap_pkt_t pdu;

    if (inet_pton(AF_INET, COAP_SERVER, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return -1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    gcoap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    if (coap_payload(&pdu, (uint8_t *)SENSOR_DATA, strlen(SENSOR_DATA)) < 0) {
        printf("Error setting payload\n");
        return -1;
    }

    ssize_t len = coap_pkt_build(buf, &pdu);
    while (1) {
        if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
            printf("Error: unable to send request\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Request sent\n");
            break;
        }
    }

    return 0;
}