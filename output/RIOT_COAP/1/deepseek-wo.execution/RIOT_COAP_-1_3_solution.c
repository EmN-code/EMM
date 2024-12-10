
#include "net/ipv6/addr.h"
#include "xtimer.h"
#include "ztimer.h"
#include "net/sock/util.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "net/sock/udp.h"
#include "time_units.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "timex.h"
#include "mma8x5x_regs.h"
#include "net/emcute.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) == NULL) {
        printf("Error parsing server IP address\n");
        return 1;
    }

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Add the payload
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(&buf[pdu.payload_len], MESSAGE, strlen(MESSAGE));
    pdu.payload_len += strlen(MESSAGE);

    // Send the CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
