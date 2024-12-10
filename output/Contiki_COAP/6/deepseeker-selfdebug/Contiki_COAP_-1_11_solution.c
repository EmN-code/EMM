
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }

    char *payload = (char *)coap_get_payload(response);
    if (payload) {
        printf("Response received: %s\n", payload);
    } else {
        printf("Empty response payload\n");
    }
}

int main(void) {
    coap_init();

    coap_message_t request;
    coap_message_t response;

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    coap_send_request(&request, SERVER_IP, SERVER_PORT, coap_response_handler);

    while (1) {
        PROCESS_YIELD();
    }

    return 0;
}
