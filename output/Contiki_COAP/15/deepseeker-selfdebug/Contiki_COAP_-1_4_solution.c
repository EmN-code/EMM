
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-nameserver.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("Response received\n");
}

int main(void) {
    coap_context_t *ctx;
    coap_address_t server_address;
    coap_message_t request;

    // Initialize CoAP context
    ctx = coap_new_context(NULL);
    if (!ctx) {
        printf("Failed to initialize CoAP context\n");
        return -1;
    }

    // Set server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.addr.sin.sin_family = AF_INET;
    server_address.addr.sin.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.addr.sin.sin_addr);

    // Create CoAP request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    // Send CoAP request
    coap_send_request(ctx, &request, &server_address, coap_response_handler);

    // Main loop
    while (1) {
        coap_run_once(ctx, 0);
    }

    return 0;
}
