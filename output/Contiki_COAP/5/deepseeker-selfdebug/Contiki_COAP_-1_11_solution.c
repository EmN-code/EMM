
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
#include "net/coap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

int main(void) {
    coap_message_t request;
    coap_message_t response;
    uint8_t buffer[128];
    int32_t offset = 0;

    // Initialize the CoAP request message
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);

    // Set the URI path for the request
    coap_set_header_uri_path(&request, COAP_URI_PATH);

    // Set the payload for the request
    const char *payload = "Hello, COAP Cloud";
    coap_set_payload(&request, payload, strlen(payload));

    // Send the CoAP request and handle the response
    coap_handler_status_t status = coap_call_handlers(&request, &response, buffer, sizeof(buffer), &offset);

    if (status == COAP_HANDLER_STATUS_OK) {
        printf("CoAP request sent successfully.\n");
    } else {
        printf("Failed to send CoAP request.\n");
    }

    return 0;
}
