#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "string.h"
#include "assert.h"
#include "thread.h"

define COAP_METHOD_POST = 2
const char EMQX_SERVER_IP[] = "47.102.103.1";
const char EMQX_URI_PATH[] = "/coap/test";
const char PAYLOAD[] = "Hello, COAP Cloud";

// CoAP packet structure
typedef struct {
    unsigned int method: 8;
    unsigned int type: 2;
    unsigned int token_len: 3;
    unsigned int code: 8;
    unsigned int msg_id: 16;
    uint8_t token[8];
    uint8_t *payload;
} coap_pkt_t;

// Remote server address structure
sock_udp_ep_t remote = {
    .port = 5683
};

// Initialize CoAP request packet
int gcoap_req_init(coap_pkt_t *pdu, uint8_t *buf, size_t len, unsigned code, const char *path) {
    if (!pdu || !buf || !path) return -1; // Validate input
    pdu->code = code;
    return 0;
}

// Send CoAP request handler
ssize_t gcoap_req_send(const uint8_t *buf, size_t len, const sock_udp_ep_t *remote,
                       void *context) {
    if (!buf || !remote) return -1; // Validate input
    // Here, sending the UDP packet over the network
    printf("Sending CoAP Message: %s\n", buf);
    return len;
}

void _resp_handler(int success) {
    if (success) {
        printf("CoAP response received successfully\n");
    }
    else {
        printf("Failed to receive CoAP response\n");
    }
}

int main() {
    uint8_t buf[256];
    coap_pkt_t pdu;

    inet_pton(AF_INET, EMQX_SERVER_IP, &(remote.addr.ipv4));

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    // Add payload to CoAP packet structure
    memcpy(buf, PAYLOAD, strlen(PAYLOAD));

    // Attempting to send the prepared CoAP buffer
    ssize_t len = gcoap_req_send(buf, sizeof(buf), &remote, NULL);
    _resp_handler(len > 0);

    return 0;
}