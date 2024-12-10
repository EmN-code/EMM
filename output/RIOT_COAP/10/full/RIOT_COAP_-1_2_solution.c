#include "ztimer.h"
#include "timex.h"
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (pdu != NULL) {
        printf("Response received: %s\n", (char *)pdu->payload);
    } else {
        printf("Request timeout or error\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t len;
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, COAP_SERVER_ADDR);

    while (1) {
        if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH) < 0) {
            printf("Failed to initialize request\n");
            xtimer_sleep(1);
            continue;
        }

        if (coap_put_option(&pdu, COAP_OPT_CONTENT_FORMAT, COAP_FORMAT_TEXT) < 0) {
            printf("Failed to set options\n");
            xtimer_sleep(1);
            continue;
        }

        len = coap_put_payload(&pdu, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

        int rc = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        if (rc < 0) {
            printf("Failed to send request: %d\n", rc);
            xtimer_sleep(1);
            continue;
        } else {
            printf("Request sent, waiting for response...\n");
        }

        xtimer_sleep(10); // Wait for some time before sending the next request
    }

    return 0;
}