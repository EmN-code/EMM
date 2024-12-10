#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "contiki-default-conf.h"
#include "coap-request-state.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
    const uint8_t *payload;
    int len = coap_get_payload(response, &payload);
    printf("Response: %.*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    static coap_endpoint_t server_endpoint;
    static coap_message_t request[1];

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_endpoint);
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

    PROCESS_END();
}