
#include <stdio.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void coap_send_post_request(void) {
    coap_message_t request;
    coap_message_t response;
    uint8_t buffer[128];
    int32_t offset = 0;

    // Initialize the CoAP request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, COAP_URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    // Send the CoAP request
    coap_call_handlers(&request, &response, buffer, sizeof(buffer), &offset);
}

int main(void) {
    // Initialize the network stack
    uip_init();

    // Send the CoAP POST request
    coap_send_post_request();

    return 0;
}
