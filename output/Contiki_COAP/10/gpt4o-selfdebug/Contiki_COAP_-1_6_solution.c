#include net/ipv6/uiplib.h
#include net/ipv6/uip-udp-packet.h
#include net/ipv6/tcp-socket.h
#include net/ipv6/udp-socket.h
#include net/ipv6/uip-icmp6.h
#include net/ipv6/udp-socket.h

#include <string.h>
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

void response_handler(coap_message_t *response) {
    const uint8_t *chunk;
    int len = coap_get_payload(response, &chunk);
    printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];  // Array of size 1 to avoid pointer issues
    static struct etimer et;

    PROCESS_BEGIN();

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    etimer_set(&et, CLOCK_SECOND * 10);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        printf("Sending CoAP POST request\n");

        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, "/coap/test");
        const char *msg = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, strlen(msg));

        COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);
        etimer_reset(&et);
    }
    
    PROCESS_END();
}
