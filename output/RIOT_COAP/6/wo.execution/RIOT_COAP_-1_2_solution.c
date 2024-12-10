#include "thread_config.h"
#include "thread.h"
#include "sock_util.h"
#include "sock/udp.h"
#include "nanocoap/cache.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "esp_wifi.h"
#include "event/thread.h"

#define ENABLE_DEBUG 1
#include "debug.h"

static gcoap_listener_t _listener;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;  // Suppress unused variable warning
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("timeout
");
        return;
    }
    if (pdu == NULL) {
        printf("reset
");
        return;
    }

    printf("Response code: %1u.%02u
", coap_get_code_class(pdu), coap_get_code_detail(pdu));
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote; // unchanged variable name

    remote.family = AF_INET;
    remote.port = 5683;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr);

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    len += coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending CoAP request: %d\n", (int)res);
    }
    return 0;
}