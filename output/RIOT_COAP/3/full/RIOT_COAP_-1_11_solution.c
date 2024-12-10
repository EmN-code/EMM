#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mutex.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "dht.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "time_units.h"
#include "assert.h"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .port = 5683 };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    const char *PAYLOAD = "Hello, COAP Cloud";
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    ssize_t res = gcoap_req_send(buf, CONFIG_GCOAP_PDU_BUF_SIZE, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error: Failed to send CoAP request\n");
    }

    xtimer_sleep(5);
    return 0;
}