
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "arpa/inet.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define RETRY_WAIT_INTERVAL 1
#define EPHEMERAL_PORT 0

static ssize_t _resp_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *context) {
    (void)pdu;
    (void)context;
    printf("Response received: %.*s\n", (int)len, (char *)buf);
    return 0;
}

int main(void) {
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int rc;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    while (1) {
        rc = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc < 0) {
            printf("Failed to send CoAP request, rc=%d\n", rc);
            xtimer_sleep(RETRY_WAIT_INTERVAL);
            continue;
        }
        printf("CoAP request sent successfully.\n");
        break;
    }

    return 0;
}