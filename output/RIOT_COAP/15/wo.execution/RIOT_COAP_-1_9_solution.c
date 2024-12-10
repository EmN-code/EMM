#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <dht.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

typedef struct {
    gcoap_request_memo_t memo;
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
} coap_payload_t;

void response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                      const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_COMPLETE) {
        printf("Response received: %.*s\n", pdu->payload_len, (char *)pdu->payload);
    }
}

int main(void) {
    coap_payload_t coap;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    gcoap_req_init(&coap.pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(coap.pdu.payload, PAYLOAD, strlen(PAYLOAD));

    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    ssize_t res = gcoap_req_send(buf, coap.pdu.hdr_len + coap.pdu.payload_len, &remote, response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending CoAP request\n");
    }

    return 0;
}