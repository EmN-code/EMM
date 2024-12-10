
#include <stdio.h>
#include <string.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/coap.h"  // Include the correct CoAP header file

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void coap_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t buffer_size, int32_t *offset) {
    // Handle CoAP response here if needed
}

int main(void) {
    coap_endpoint_t server;
    coap_message_t request;
    uint8_t payload[] = "Hello, COAP Cloud";
    uint8_t buffer[128];
    int32_t offset = 0;

    // Initialize CoAP endpoint
    coap_endpoint_init(&server, COAP_SERVER_IP, COAP_SERVER_PORT);

    // Prepare CoAP POST request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, COAP_URI_PATH);
    coap_set_payload(&request, payload, sizeof(payload) - 1);

    // Send CoAP request
    coap_call_handlers(&request, NULL, buffer, sizeof(buffer), &offset);

    return 0;
}
