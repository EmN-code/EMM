#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "stdbool.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "dht_params.h"
#include "net/emcute.h"
#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"

static ssize_t _resp_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len) {
    (void)buf;
    (void)len;
    printf("Response received: Code %u\n", coap_get_code(pdu));
    return 0;
}

int main(void) {
    static char message[] = "Hello, COAP Cloud";
    static const char EMQX_SERVER_IP[] = "47.102.103.1";
    static const char URI_PATH[] = "/coap/test";
    unsigned msg_count = 0;
    size_t len;
    static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int result;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    len += coap_payload(&pdu, (uint8_t *)message, strlen(message));

    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: invalid server address\n");
        return -1;
    }

    remote.port = 5683;
    result = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        printf("Error: message send failed\n");
        return -1;
    }

    printf("CoAP message sent: %s\n", message);

    return 0;
}