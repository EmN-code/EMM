
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

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    // Response handler logic can be added here
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Convert IP address string to binary form
    inet_aton(SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Add payload to the CoAP request
    coap_add_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    // Finish the CoAP options
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Send the CoAP request
    gcoap_req_send(buf, pdu.offset, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
