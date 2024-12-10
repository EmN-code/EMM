
#include <stdio.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <sys/socket.h>
#include <ztimer.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <msg.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <time_units.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        printf("Received response with code %02u.%02u\n",
               coap_get_code_class(pdu->hdr->code),
               coap_get_code_detail(pdu->hdr->code));
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, payload, payload_len);

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Invalid IP address");
        return 1;
    }

    gcoap_req_send(buf, pdu.offset, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
