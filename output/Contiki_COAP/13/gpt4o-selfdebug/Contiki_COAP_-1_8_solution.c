#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/mac/framer/framer.h"
#include "contiki-default-conf.h"

#include <stdio.h>
#include "coap-engine.h"

#define REMOTE_PORT 5683
#define REMOTE_ADDR "47.102.103.1"

static coap_endpoint_t server_endpoint;
static coap_message_t request[1];

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

void response_handler(coap_message_t *response) {
    const uint8_t *chunk;
    int len = coap_get_payload(response, &chunk);
    printf("Response: %*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    printf("CoAP Client Process Started\n");

    coap_endpoint_parse(REMOTE_ADDR, REMOTE_PORT, &server_endpoint);
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char *msg = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, strlen(msg));

    COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

    PROCESS_END();
}

int main(void) {
    process_start(&coap_client_process, NULL);
    return 0;
}