#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "string.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "assert.h"
#include "net/emcute.h"
#include "thread.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _on_response(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Error: response timeout\n");
        return;
    }

    if (pdu == NULL) {
        printf("Error: invalid response\n");
        return;
    }

    printf("Response received: ");
    int payload_len = coap_get_payload_len(pdu);
    printf("%.*s\n", payload_len, (char *)coap_get_payload(pdu));
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    int res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        printf("Error: invalid server IP\n");
        return 1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    pdu.payload_len = strlen(COAP_PAYLOAD);

    // Send CoAP request
    ssize_t sent = gcoap_req_send(buf, pdu.payload_len, &remote, _on_response, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (sent <= 0) {
        printf("Error: failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");

    return 0;
}