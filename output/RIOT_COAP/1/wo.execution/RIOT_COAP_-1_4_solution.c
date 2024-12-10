#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include <arpa/inet.h>

#define COAP_METHOD_POST 2  // POST method
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define PAYLOAD "Hello, COAP Cloud"
#define CONFIG_GCOAP_PDU_BUF_SIZE 256  // define size according to your needs

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;  // Unused
    (void)pdu;   // Unused
    (void)remote; // Unused
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t result;

    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    if (result != 1) {
        puts("Failed to convert IP address");
        return -1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    result = gcoap_req_send(buf, coap_get_total_hdr_len(&pdu) + strlen(PAYLOAD), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (result <= 0) {
        puts("Failed to send request");
        return -1;
    }

    return 0;
}