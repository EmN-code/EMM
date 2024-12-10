# Required headers for functionality
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "string.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "ztimer.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "assert.h"

#define COAP_SERVER_IAddress "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static ssize_t _response_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, const sock_udp_ep_t *remote){

    (void)remote;
    (void)len;
    (void)buf;

    if (pdu == NULL) {
        puts("Error: timeout" );
        return -1;
    }

    printf("Response received: %s\n", (char *)pdu->payload);

    return 0;
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    inet_pton(AF_INET, COAP_SERVER_IAddress, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    ssize_t result = gcoap_req_send(buf, pdu.payload_len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if(result < 0){
        puts("Error sending CoAP request");
    } else {
        puts("CoAP message sent");
    }

    return 0;
}