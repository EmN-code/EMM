#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ns6-route.h"
#include "net/ipv6/uip-ns6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/tcp-socket.h"
#include "coap-request-state.h"
#include <contiki.h>
#include <stdio.h>

PROCESS(coap_post_example, "CoAP POST Example");
AUTOSTART_PROCESSES(&coap_post_example);

static void response_handler(coap_message_t *response) {
    const uint8_t *payload;
    int len = coap_get_payload(response, &payload);
    printf("Response: %.*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_post_example, ev, data) {
    PROCESS_BEGIN();

    static coap_endpoint_t server_ep;
    static coap_message_t request[1];
    uip_ip6addr(&server_ep.ipaddr, 0x2F, 0x66, 0x67, 0x01); // Convert the IP to binary
    server_ep.port = UIP_HTONS(5683);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");

    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

    PROCESS_END();
}