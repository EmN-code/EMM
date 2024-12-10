
#include "coap-blocking-request.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "coap-request-state.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void chunk_handler(coap_message_t *response) {
    if(response == NULL) {
        printf("Request timed out\n");
        return;
    }

    uint8_t *payload = NULL;
    size_t payload_len = 0;
    coap_get_payload(response, &payload, &payload_len);

    if(payload_len > 0) {
        printf("Received: %.*s\n", (int)payload_len, payload);
    }
}

int main(void) {
    coap_endpoint_t server_endpoint;
    coap_request_t request;

    // Initialize the CoAP endpoint
    coap_endpoint_init(&server_endpoint, SERVER_IP, SERVER_PORT);

    // Prepare the CoAP request
    coap_init_request(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    // Send the CoAP request
    COAP_BLOCKING_REQUEST(&server_endpoint, &request, chunk_handler);

    return 0;
}
