
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"

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

    coap_endpoint_t server;
    coap_endpoint_init(&server, UIP_ADDR_ANY, 0, 0);
    uip_ipaddr_t server_addr;
    uip_ipaddr(&server_addr, SERVER_IP);
    coap_endpoint_connect(&server, &server_addr, SERVER_PORT);

    coap_message_t request;
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    coap_set_payload(&request, (uint8_t *)payload, strlen(payload));

    coap_send_request(&server, &request, coap_response_handler);

    while (1) {
        coap_process();
    }

    return 0;
}
