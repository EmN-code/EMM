#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    /* Initialize socket */
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    inet_pton(AF_INET, COAP_SERVER_ADDR, &remote.addr.ipv4);

    /* Initialize CoAP request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    /* Set payload */
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    pdu.payload_len = strlen(COAP_MESSAGE);

    /* Send CoAP request */
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending request\n");
    }

    return 0;
}