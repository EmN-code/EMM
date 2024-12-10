#include "ztimer.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "msg.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "timex.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "stddef.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "net/af.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "unistd.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void on_response(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Response handler (not implementing details as it depends on response logic)
    printf("Response received\n");
}

int main(void) {
    // Define endpoint for the CoAP request
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    // Convert the server IP address
    while (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Invalid IP address format. Retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    // CoAP buffer setup
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    unsigned code = COAP_METHOD_POST;

    // Initialize CoAP request, retry if failed
    while (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, code, COAP_URI_PATH) < 0) {
        printf("Failed to initialize CoAP request. Retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    // Copy message into CoAP payload
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    pdu.payload_len = strlen(COAP_MESSAGE);

    // Send CoAP request
    while (gcoap_req_send(buf, pdu.payload_len, &remote, on_response, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Failed to send CoAP request. Retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}