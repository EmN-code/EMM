
#include <stdio.h>
#include <string.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_post_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("Response received\n");
}

int main(void) {
    coap_endpoint_t server;
    coap_message_t request;
    coap_message_t response;
    uint8_t buffer[128];
    uint16_t buffer_size = sizeof(buffer);

    // Initialize the CoAP endpoint
    coap_endpoint_init(&server, SERVER_IP, SERVER_PORT);

    // Create a CoAP POST request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Send the CoAP request
    coap_send_request(&server, &request, &response, buffer, buffer_size, coap_post_handler);

    return 0;
}
