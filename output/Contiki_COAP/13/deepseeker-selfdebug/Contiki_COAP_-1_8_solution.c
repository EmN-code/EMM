
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void coap_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t buffer_size, int32_t *offset) {
    // Handle CoAP response here
}

int main(void) {
    // Initialize CoAP
    coap_init();

    // Create a CoAP message
    coap_message_t request;
    coap_message_init(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_add_option(&request, COAP_OPTION_URI_PATH, strlen(COAP_URI_PATH), (uint8_t *)COAP_URI_PATH);
    coap_set_payload(&request, (uint8_t *)"Hello, COAP Cloud", 16);

    // Send the CoAP request
    coap_send(&request, COAP_SERVER_IP, COAP_SERVER_PORT, coap_handler);

    // Main loop
    while (1) {
        // Process CoAP events
        coap_process();
    }

    return 0;
}
