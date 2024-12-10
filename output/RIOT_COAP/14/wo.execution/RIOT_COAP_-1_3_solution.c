#include "thread_config.h"
#include "thread.h"
#include "sys/socket.h"
#include "stddef.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/nanocoap/cache.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 0x02
#define GCOAP_SOCKET_TYPE_UDP 0
#define COAP_OPT_FINISH_NONE 0

static void _resp_handler(const coap_pkt_t *pdu, const sock_udp_ep_t *ep, void *context) {
    (void)pdu;
    (void)ep;
    (void)context;
    printf("Response received");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = 5683 };
    inet_pton(AF_INET6, "::FFFF:47.102.103.1", &remote.addr.ipv6);

    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test") == 0) {
        printf("PDU initialized\n");
    }

    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    const char *payload = "Hello, COAP Cloud";
    coap_opt_add_payload(&pdu, len, (uint8_t*)payload, strlen(payload));

    ssize_t res = gcoap_req_send(buf, len + strlen(payload), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res > 0) {
        printf("Request sent\n");
    } else {
        printf("Failed to send request\n");
    }

    return 0;
}