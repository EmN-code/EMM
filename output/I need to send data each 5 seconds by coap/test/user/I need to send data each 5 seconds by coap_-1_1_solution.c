
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"
#define SEND_INTERVAL (5 * US_PER_SEC)  // 5 seconds interval

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char resp_buf[GCOAP_PDU_BUF_SIZE];
    size_t len = gcoap_response_phrase(coap_get_code(pdu), resp_buf);
    printf("CoAP status: %s\n", resp_buf);
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = COAP_SERVER_PORT;
    remote.family = AF_INET;

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, COAP_SERVER_IP) == NULL) {
        puts("Error: unable to parse destination address");
        return 1;
    }

    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
        coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
        ssize_t payload_len = coap_payload_start(&pdu);
        memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
        coap_finish(&pdu, payload_len + strlen(COAP_MESSAGE), COAP_NO_ERROR);

        gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        xtimer_periodic_wakeup(&last_wakeup, SEND_INTERVAL);
    }

    return 0;
}
