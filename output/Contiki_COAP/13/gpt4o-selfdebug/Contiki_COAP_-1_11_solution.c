#include <net/ipv6/uiplib.h>
#include <net/ipv6/uip-sr.h>
#include <net/ipv6/uip-nameserver.h>
#include <net/ipv6/uip-lib.h>
#include <net/ipv6/uip-ds6-route.h>
#include <net/ipv6/uip-ds6-nbr.h>
#include <net/ipv6/uip-icmp6.h>
#include <net/ipv6/uip-udp-packet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COAP_BLOCKING_REQUEST(server_endpoint, request, chunk_handler)

int main() {
    const char *server_ip = "47.102.103.1";
    const uint16_t server_port = 5683;
    const char *uri_path = "/coap/test";
    const char *payload = "Hello, COAP Cloud";
    uint8_t buffer[256];
    uint16_t buffer_size = sizeof(buffer);

    printf("Preparing CoAP POST request to %s:%d%s\n", server_ip, server_port, uri_path);

    // Construct the CoAP message
    coap_message_t request;
    coap_udp_init_message(&request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(&request, uri_path);
    coap_set_payload(&request, (uint8_t *)payload, strlen(payload));

    // Set up the CoAP server endpoint
    coap_endpoint_t server_endpoint;
    coap_endpoint_parse(server_ip, server_port, &server_endpoint);

    // Send the CoAP POST Request
    int result = coap_send_request(&server_endpoint, &request);

    if (result) {
        printf("CoAP POST request successfully sent\n");
    } else {
        printf("Failed to send CoAP POST request\n");
    }

    return 0;
}