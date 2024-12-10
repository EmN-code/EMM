// Solution to the problem for sending a CoAP POST request using RIOT on ESP32

// Importing required header files in descending alphabetical order
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <time_units.h>
#include <string.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <msg.h>
#include <esp_wifi.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <assert.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 256
#define COAP_METHOD_POST 2
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_PORT 5683
#define PAYLOAD "Hello, COAP Cloud"

typedef struct {
    sock_udp_ep_t remote;
    size_t payload_len;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
} coap_request_t;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (!pdu) {
        puts("Error: No response received");
        return;
    }
    char resp[64];
    memcpy(resp, (char *)pdu->payload, pdu->payload_len);
    resp[pdu->payload_len] = '\0';
    printf("Response: %s\n", resp);
}

void send_coap_request(void) {
    coap_request_t request;
    request.remote.family = AF_INET;
    request.remote.port = EMQX_PORT;
    inet_pton(AF_INET, EMQX_SERVER_IP, &request.remote.addr.ipv4);
    
    request.payload_len = gcoap_req_init(&request.pdu, request.buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(request.pdu.payload, PAYLOAD, strlen(PAYLOAD));
    request.pdu.payload_len = strlen(PAYLOAD);
    
    ssize_t bytes_sent = gcoap_req_send(request.buf, request.payload_len, &request.remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        puts("Failed to send request");
    }
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    send_coap_request();
    return 0;
}