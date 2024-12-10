
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <xtimer.h>
#include <sys/socket.h>
#include <ztimer.h>
#include <msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <stdatomic.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <dht.h>
#include <timex.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <net/ipv6/addr.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request error");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received: %d\n", (int)pdu->payload_len);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Invalid IP address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    coap_hdr_set_code(pdu.hdr, COAP_CODE_POST);
    coap_hdr_set_token(pdu.hdr, (uint8_t *)"token", 5);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_add_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));
    size_t len = coap_get_total_bytes(&pdu);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
