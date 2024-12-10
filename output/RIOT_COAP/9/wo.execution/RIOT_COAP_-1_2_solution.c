#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_METHOD COAP_METHOD_POST

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received: %u \n", pdu->payload[0]);
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len;

    sock_udp_ep_t remote = {
            .family = AF_INET6,
            .netif = SOCK_ADDR_ANY_NETIF,
            .port = SERVER_PORT
    };

    printf("Sending CoAP POST request\n");

    /* initialize the coap request */
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD, COAP_URI_PATH);
    if (len <= 0) {
        printf("Error initializing CoAP request\n");
        return 1;
    }

    /* set the message payload */
    if (coap_opt_add_format(&pdu, COAP_FORMAT_TEXT) == 0) {
        printf("Error adding content format option\n");
        return 1;
    }

    const char *payload = "Hello, COAP Cloud";
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, payload, strlen(payload));
    coap_hdr_set_payload_len(&pdu, strlen(payload));

    /* send the request */
    if (gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    return 0;
}