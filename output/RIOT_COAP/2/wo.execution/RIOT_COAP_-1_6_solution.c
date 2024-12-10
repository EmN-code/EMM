#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "time_units.h"

#define SERVER_IP           "47.102.103.1"
#define SERVER_PORT         5683
#define COAP_URI_PATH       "/coap/test"
#define BUFFER_SIZE         128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout from server\n");
    } else {
        printf("Response received from server\n");
    }
}

int main(void) {
    uint8_t buf[BUFFER_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    ssize_t bytes = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (bytes < 0) {
        printf("Error initializing CoAP request\n");
        return 1;
    }

    gcoap_req_send(buf, bytes, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        xtimer_sleep(5);
    }

    return 0;
}