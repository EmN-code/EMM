#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/simple-udp.h"
#include "contiki-default-conf.h"
#include "coap-request-state.h"
#include <stdio.h>
#include <string.h>

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_POST_URI "/coap/test"

static void response_handler(coap_message_t *response) {
    const uint8_t *payload;
    if (response == NULL) {
        printf("Request timed out\n");
        return;
    }
    int len = coap_get_payload(response, &payload);
    printf("Response: %.*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();
    
    static coap_packet_t request[1];
    
    printf("CoAP Client Example\n");

    // Prepare the CoAP POST request
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, COAP_POST_URI);

    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    uip_ipaddr_t server_ipaddr;
    uip_ip6addr(&server_ipaddr, 0x47, 0x66, 0x67, 0x01, 0, 0, 0, 0);

    COAP_BLOCKING_REQUEST(&server_ipaddr, COAP_SERVER_PORT, request, response_handler);

    PROCESS_END();
}