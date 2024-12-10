#include "timex.h"
#include "time_units.h"
#include "xtimer.h"
#include "ztimer.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "esp_wifi.h"
#include "arpa/inet.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_METHOD COAP_METHOD_POST
#define PAYLOAD "Hello, COAP Cloud"
#define GCOAP_PDU_BUF_SIZE 128  // Ensure this constant is properly defined

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Received response\n");
}

int main(void) {
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    size_t len;
    sock_udp_ep_t remote;

    // Configure the remote endpoint
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    // Initialize the CoAP packet
    coap_pkt_t pdu;
    len = gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD, COAP_URI_PATH);

    // Add payload to the packet
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    xtimer_sleep(1); // Wait for a while to receive response

    return 0;
}