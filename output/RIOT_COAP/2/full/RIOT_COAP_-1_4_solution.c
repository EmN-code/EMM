#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "dht_params.h"
#include "MQTTClient.h"
#include "arpa/inet.h"

#define COAP_SERVER_IP      "47.102.103.1"
#define COAP_SERVER_PORT    5683
#define COAP_URI_PATH       "/coap/test"
#define COAP_MESSAGE        "Hello, COAP Cloud"

static sock_udp_t sock;
static sock_udp_ep_t remote;

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) memo;
    (void) remote;
    /* handle response, if needed */
}

void send_coap_request(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int res;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, sizeof(COAP_MESSAGE));
    pdu.payload_len = sizeof(COAP_MESSAGE);

    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.port = COAP_SERVER_PORT;

    res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res <= 0) {
        /* handle error in sending */
    }
}

int main(void) {
    sock_udp_create(&sock, NULL, NULL, 0);

    send_coap_request();

    while (1) {
        xtimer_sleep(5);
        send_coap_request();
    }

    return 0;
}