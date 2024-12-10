
#include <stdio.h>
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/coap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t buffer_size, int32_t *offset) {
    // Handle CoAP response if needed
}

int main(void) {
    udp_socket_t socket;
    coap_resource_t resource;
    coap_message_t request, response;
    uint8_t buffer[128];
    int32_t offset = 0;

    // Initialize the CoAP resource
    coap_activate_resource(&resource, URI_PATH);

    // Initialize the UDP socket
    udp_socket_init(&socket);
    udp_socket_bind(&socket, SERVER_PORT);

    // Prepare the CoAP request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_add_payload(&request, (uint8_t *)"Hello, COAP Cloud", 16);

    // Send the CoAP request
    coap_send(&request, &socket, SERVER_IP, SERVER_PORT);

    // Receive and handle the CoAP response
    coap_receive(&socket, buffer, sizeof(buffer));
    coap_call_handlers(&request, &response, buffer, sizeof(buffer), &offset);

    return 0;
}
