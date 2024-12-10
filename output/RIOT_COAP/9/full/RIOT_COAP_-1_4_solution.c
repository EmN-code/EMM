#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "esp_system.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "net/emcute.h"
#include "arpa/inet.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;  // Not used
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("CoAP request timeout");
        return;
    }
    if (memo->state == GCOAP_MEMO_ERR) {
        puts("CoAP request error");
        return;
    }
    char *payload = (char *)coap_payload(pdu);
    printf("Response payload: %s\n", payload);
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t res;

    // Convert IP address from string representation
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    while (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error in IP address conversion. Retrying...\n");
        xtimer_usleep(1000000);
    }

    // Initialize CoAP request
    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (len < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    memcpy(coap_payload(&pdu), COAP_MESSAGE, strlen(COAP_MESSAGE));
    gcoap_opt_finish(&pdu, len, COAP_OPTION_BLOCK2, COAP_MESSAGE_BLOCK_SIZE);

    // Send request and check for errors
    while ((res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP)) < 0) {
        printf("CoAP request send failed: %d\n", (int)res);
        xtimer_usleep(1000000);
    }

    printf("CoAP POST request sent successfully\n");
    return 0;
}