
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <esp_system.h>
#include <ztimer.h>
#include <esp_wifi.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    // Response handling code here
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        return 1; // Error handling
    }

    // Initialize CoAP request
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH) < 0) {
        return 1; // Error handling
    }

    // Add payload to the CoAP request
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    if (coap_opt_add_format(&pdu, COAP_FORMAT_TEXT_PLAIN) < 0) {
        return 1; // Error handling
    }
    if (coap_add_payload(&pdu, (uint8_t *)payload, payload_len) < 0) {
        return 1; // Error handling
    }

    // Finish the CoAP request
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE) < 0) {
        return 1; // Error handling
    }

    // Send the CoAP request
    if (gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        return 1; // Error handling
    }

    return 0;
}
