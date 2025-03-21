
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <time_units.h>
#include <esp_system.h>
#include <xtimer.h>
#include <mutex.h>
#include <ztimer.h>
#include <esp_wifi.h>
#include <kernel_defines.h>
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
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = HTONS(EMQX_SERVER_PORT);

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Failed to convert IP address\n");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    coap_hdr_set_payload(&pdu.hdr, (uint8_t *)"Hello, COAP Cloud", 16);

    ssize_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (len < 0) {
        printf("Failed to finish CoAP options\n");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
