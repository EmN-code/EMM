#include "xtimer.h"
#include "net/gcoap.h"
#include <stdio.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"

static void coap_response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    // Basic response handler
    (void)memo;
    (void)remote;
    if (pdu) {
        printf("Received response with code %u\n", coap_get_code_class(pdu) * 100 + coap_get_code_detail(pdu));
    }
    else {
        printf("No response received.\n");
    }
}

int main(void)
{
    static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = {
        .family  = AF_INET,
        .port    = COAP_SERVER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, COAP_SERVER_IP);

    while (1) {
        int rc = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
        if(rc < 0) {
            printf("Failed to initialize CoAP request\n");
            xtimer_sleep(1);
            continue;
        }

        size_t length = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

        memcpy(pdu.payload, "Hello, COAP Cloud", 17);
        length += 17;

        rc = gcoap_req_send(buf, length, &remote, coap_response_handler, NULL);
        if (rc <= 0) {
            printf("Failed to send CoAP request\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("CoAP request sent\n");
        }

        xtimer_sleep(10); // Send every 10 seconds
    }

    return 0;
}