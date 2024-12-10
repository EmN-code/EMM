
#include <evtimer_msg.h>
#include <net/ipv6/addr.h>
#include <ztimer.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <xtimer.h>
#include <esp_wifi.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <timex.h>
#include <time_units.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void) {
    sock_udp_ep_t remote = SOCK_IPV4;
    remote.port = SERVER_PORT;

    if (sock_udp_name2ep(&remote, SERVER_IP, NULL) < 0) {
        printf("Error parsing server address\n");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_add_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    if (gcoap_req_send(buf, coap_get_total_hdr_len(&pdu) + coap_get_payload_len(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}
