// Required Libraries in Descending Order
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "arpa/inet.h"

// Definitions
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received.\n");
}

int main(void) {
    // Initialize CoAP endpoint and UDP sock
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP request
    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    size_t payload_len = strlen(COAP_PAYLOAD);

    // Send CoAP request
    gcoap_req_send(buf, len + payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Main loop
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}