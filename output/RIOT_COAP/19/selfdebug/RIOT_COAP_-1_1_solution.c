#include <stdio.h>
#include <net/nanocoap/cache.h>
#include <thread_config.h>

#include <xtimer.h>
#include <net/gcoap.h>

#define SERVER_EP       "coap://47.102.103.1"
#define SERVER_PORT     CONFIG_GCOAP_PORT
#define SERVER_PATH     "/coap/test"

static uint8_t _buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static sock_udp_ep_t remote = {
    .family = AF_INET6,  // Configure AF_INET6 for compatibility with CoAP IPv6 specs
    .port = SERVER_PORT
};

static const char *message = "Hello, COAP Cloud";

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;  // Unused
    if (memo->state == GCOAP_MEMO_RESP_TRUNC) {
        printf("Error: response is too large, truncated\n");
    }
    else if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Error: response timeout\n");
    }
    else {
        printf("Success: received response with code %u \n", coap_get_code(pdu));
    }
}

int main(void) {
    size_t len;
    coap_pkt_t pdu;

    printf("Starting CoAP client example\n");

    // Initialize GCOAP request
    len = gcoap_req_init(&pdu, _buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, SERVER_PATH);
    memcpy(pdu.payload, message, strlen(message));
    len += strlen(message);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Send GCOAP request
    remote.port = SERVER_PORT;
    gcoap_req_send(_buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("Sent request to %s:%d\n", SERVER_EP, SERVER_PORT);

    // Sleep briefly to yield time for processing response
    xtimer_sleep(5);

    return 0;
}
