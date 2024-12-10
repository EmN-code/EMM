
#include <stdio.h>
#include "net/ipv6/uip-ds6.h"
#include "net/coap.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("CoAP response received\n");
}

int main(void) {
    uip_ipaddr_t server_ip;
    coap_endpoint_t server;
    coap_message_t request;

    // Initialize the CoAP request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, COAP_URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    // Set the server IP address and port
    uip_ipaddr(&server_ip, COAP_SERVER_IP);
    coap_endpoint_init(&server, &server_ip, UIP_HTONS(COAP_SERVER_PORT), 0);

    // Send the CoAP request
    coap_send_request(&request, &server, coap_response_handler);

    // Main loop
    while (1) {
        // Process CoAP events
        coap_process();
    }

    return 0;
}
