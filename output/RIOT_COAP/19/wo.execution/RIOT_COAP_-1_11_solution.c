#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "msg.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "stdbool.h"
#include "unistd.h"
#include "string.h"
#include "stdio.h"
#include "dht_params.h"
#include "dht.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "stdlib.h"
#include "stddef.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "stdatomic.h"
#include "net/emcute.h"
#include "netinet/in.h"
#include "time_units.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE (128U)  // Define it once

static ssize_t gcoap_request_handler(const uint8_t *pdu, size_t len,
                                     const sock_udp_ep_t *remote,
                                     void *context, gcoap_socket_type_t type) {
    (void)context;
    (void)type;
    puts("CoAP request sent successfully");
    return len;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    coap_pkt_t pdu;
    const char *PAYLOAD = "Hello, COAP Cloud";

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    gcoap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    gcoap_req_send(buf, pdu.hdr.len, &remote, gcoap_request_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}