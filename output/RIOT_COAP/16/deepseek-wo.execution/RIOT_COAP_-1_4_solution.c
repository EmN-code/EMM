
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "time_units.h"
#include "xtimer.h"
#include "esp_system.h"
#include "mutex.h"
#include "ztimer.h"
#include "esp_wifi.h"
#include "kernel_defines.h"
#include "net/sock/util.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "mbox.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    // Response handling code here
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Convert the IP address string to a byte representation
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        // Handle error
        return 1;
    }

    // Add payload to the CoAP request
    coap_add_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP request
    gcoap_req_send(buf, pdu.offset, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
