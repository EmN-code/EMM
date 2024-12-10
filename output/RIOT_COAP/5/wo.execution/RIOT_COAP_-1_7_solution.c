#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <esp_wifi.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>

#define BUF_SIZE    (128)
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Response timeout\n");
        return;
    }
    printf("Received response:\n");
    printf("%.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
}

int main(void) {
    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;
    size_t len;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse destination address\n");
        return -1;
    }

    len = gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    memcpy(pdu.payload, "Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);
    len += sizeof("Hello, COAP Cloud") - 1;

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: unable to send request\n");
    }

    while (1) {
        xtimer_sleep(5);
    }

    return 0;
}
