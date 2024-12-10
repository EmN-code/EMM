#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

#define RETRY_INTERVAL (1U * US_PER_SEC)

static const sock_udp_ep_t server = {
    .family = AF_INET,
    .port = COAP_SERVER_PORT
};

static ssize_t _resp_handler(coap_pkt_t *pdu, 
                              const sock_udp_ep_t *remote,
                              void *context) 
{
    (void)context;
    (void)remote;

    if (coap_get_code_class(pdu) == COAP_CLASS_SUCCESS) {
        puts("Received successful response");
    } else {
        puts("Failed response");
    }
    return 0;
}

int main(void)
{
    char buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t result;

    sock_udp_ep_t server_ep = server;
    ipv6_addr_from_str((ipv6_addr_t *)&server_ep.addr, COAP_SERVER_ADDR);

    while (1) {
        gcoap_req_init(&pdu, (uint8_t *)buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
        coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

        result = gcoap_req_send((uint8_t *)buf, pdu.payload_len, &server_ep, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (result <= 0) {
            puts("gcoap_req_send failed, trying again...");
            xtimer_sleep(RETRY_INTERVAL);
        } else {
            break;
        }
    }

    return 0;
}
