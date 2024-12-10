#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "paho_mqtt.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "coap_pkt.h"
#include "assert.h"

#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

// CoAP message payload
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response from the server
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .port = SERVER_PORT };
    
    // Convert the server IP address from text to binary form
    int result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("inet_pton failed\n");
        return -1;
    }
    
    // Initialize CoAP request
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH) < 0) {
        printf("Error initializing request\n");
        return -1;
    }

    // Add payload to CoAP packet
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    ssize_t len = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len <= 0) {
        printf("Error sending request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    
    // Keep the application alive to await responses
    while (1) {}
    
    return 0;
}