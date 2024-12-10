#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "esp_wifi.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"
#define BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("GCOAP: timeout");
        return;
    }

    if (memo->state != GCOAP_MEMO_COMPLETE) {
        puts("GCOAP: error in response");
        return;
    }

    printf("GCOAP: received %u bytes
", pdu->payload_len);
}

int main(void) {
    uint8_t buf[BUF_SIZE];
    size_t len;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_NETIF_UNSPEC;
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, COAP_MSG, sizeof(COAP_MSG) - 1);
    len += sizeof(COAP_MSG) - 1;

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    puts("CoAP request sent");

    return 0;
}
