#include "time_units.h"
#include "thread.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "msg.h"
#include "dht_params.h"
#include "dht.h"
#include "esp_wifi.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "net/emcute.h"
#include "arpa/inet.h"
#include "stdio.h"
#include "assert.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Could not convert IP address\n");
        return 1;
    }

    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (len < 0) {
        printf("Error: Could not initialize CoAP request\n");
        return 1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    ssize_t result = gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        printf("Error: Could not send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent successfully\n");

    /* Add necessary code to keep the main function running, such as a loop 
       or handling other events, since RIOT applications usually run continuously */
    while (1) {
        /* Alternatively, could handle incoming messages or other processing */
        ; 
    }

    return 0;
}