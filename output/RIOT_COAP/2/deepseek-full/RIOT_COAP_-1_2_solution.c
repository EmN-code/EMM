
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
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        puts("Request succeeded");
    }
}

int main(void)
{
    sock_udp_ep_t server = SOCK_IPV4_EP_ANY;
    server.port = SERVER_PORT;

    if (sock_udp_str2ep(&server, SERVER_IP) < 0) {
        puts("Error parsing server address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    len += strlen(MESSAGE);

    gcoap_req_send(buf, len, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
