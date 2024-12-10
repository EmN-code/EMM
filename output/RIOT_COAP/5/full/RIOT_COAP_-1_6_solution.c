#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "string.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "assert.h"

#define SERVER_PORT 5683
#define SERVER_IP "47.102.103.1"
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static ztimer_t timer;
static char buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;  // argument set but not used
    puts("Response received");
}

int main(void) {
    // WiFi and UDP Endpoint Setup
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP
    gcoap_register_listener(&_rcv_listener);

    while(1) {
        // Prepare CoAP request
        coap_pkt_t pdu;
        gcoap_req_init(&pdu, (uint8_t *)buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
        memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

        // Send CoAP request
        if (gcoap_req_send((const uint8_t *)buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) > 0) {
            puts("CoAP request sent");
        } else {
            puts("Failed to send CoAP request");
        }

        // Wait before sending the next request
        ztimer_sleep(ZTIMER_MSEC, 10000);
    }

    return 0;
}