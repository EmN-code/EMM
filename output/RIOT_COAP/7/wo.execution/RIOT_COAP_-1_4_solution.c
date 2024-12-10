#include "net/sock/udp.h"
#include "event/thread.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "esp_system.h"
#include "dht_params.h"
#include "arpa/inet.h"

#define COAP_PORT 5683

static const char *EMQX_SERVER_IP = "47.102.103.1";
#define EMQX_URI_PATH "/coap/test"
#define COAP_METHOD_POST 0x02

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    uint8_t buf[128];
    size_t len;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    coap_pkt_t pdu;
    int result;

    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        printf("Error: unable to parse IP address\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    pdu.payload_len = 17;

    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error: unable to send CoAP request\n");
        return 1;
    }

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Received CoAP response\n");
}