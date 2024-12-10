#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "msg.h"
#include "MQTTClient.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "dht.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "arpa/inet.h"
#include "assert.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static msg_t _msg_queue[8]; // Increase or configure as required

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received: %u\n", coap_get_code(pdu));
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    // Setup message queue
    msg_init_queue(_msg_queue, sizeof(_msg_queue) / sizeof(msg_t));

    // Initialize buffer and endpoint
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET6, .port = EMQX_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    // Prepare CoAP request
    coap_pkt_t pdu;
    ssize_t bytes;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_hdr_set_payload(&pdu, pdu.payload, strlen(PAYLOAD));

    // Send CoAP request
    bytes = gcoap_req_send(buf, pdu.hdrlen + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes <= 0) {
        printf("Error sending CoAP request\n");
    }

    // Implement a delay or main loop to keep the program running if necessary
    ztimer_sleep(ZTIMER_MSEC, 1000);

    return 0;
}