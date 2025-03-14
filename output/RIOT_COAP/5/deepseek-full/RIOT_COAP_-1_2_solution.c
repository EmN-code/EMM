
#include "timex.h"
#include "evtimer_mbox.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "ztimer.h"
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define REQUEST_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    } else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    } else {
        printf("Response received: %d\n", (int)pdu->payload_len);
    }
}

int main(void)
{
    sock_udp_ep_t server = SOCK_IPV4_EP_ANY;
    server.port = SERVER_PORT;
    server.family = AF_INET;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    if (sock_udp_name2ep(&server, SERVER_IP, NULL) < 0) {
        puts("Failed to resolve server address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, REQUEST_PAYLOAD, sizeof(REQUEST_PAYLOAD) - 1);
    pdu.payload_len = sizeof(REQUEST_PAYLOAD) - 1;
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    gcoap_req_send(buf, pdu.offset, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
