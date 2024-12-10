
#include "net/emcute.h"
#include "evtimer_msg.h"
#include "mma8x5x_regs.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "thread.h"
#include "esp_wifi.h"
#include "ztimer.h"
#include "kernel_defines.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "net/ipv6/addr.h"
#include "net/sock/util.h"
#include "mutex.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Add payload
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));

    // Send CoAP request
    ssize_t len = gcoap_finish(&pdu, strlen(MESSAGE), COAP_TYPE_CON);
    if (len < 0) {
        printf("Failed to finish CoAP request\n");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
