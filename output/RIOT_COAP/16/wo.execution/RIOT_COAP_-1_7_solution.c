#include <xtimer.h>
#include <ztimer.h>
#include <sys/socket.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <timex.h>
#include <time_units.h>
#include <unistd.h>
#include <esp_wifi.h>
#include <esp_system.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define COAP_METHOD_POST 0x02
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

ssize_t _resp_handler(const coap_pkt_t *pdu, void *context) {
    (void)context;
    printf("Response received\n");
    return 0;
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    size_t len;

    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);
    coap_hdr_set_type(&pdu, COAP_NON);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}