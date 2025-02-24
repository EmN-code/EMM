#include "net/ipv6/uip-ds6-route.h"
#include "net/ip/uip.h"
#include "contiki.h"
#include "coap-engine.h"

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
    const uint8_t *payload;
    int len = coap_get_payload(response, &payload);
    printf("Response: %.*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    coap_endpoint_t server_ep;
    coap_message_t request[1];  // This way the packet can be treated as pointer as usual

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    while(1) {
        printf("Sending CoAP POST request\n");

        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, "/coap/test");
        const char msg[] = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

        COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

        PROCESS_PAUSE();
    }

    PROCESS_END();
}
