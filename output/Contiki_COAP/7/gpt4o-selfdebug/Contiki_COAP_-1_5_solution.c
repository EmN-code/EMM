#include "net/ipv6/uip-udp-packet.h"
#include "coap-request-state.h"
#include "contiki-default-conf.h"
#include "net/ipv6/simple-udp.h"

PROCESS(hello_world_process, "Hello World Process");
AUTOSTART_PROCESSES(&hello_world_process);

static coap_endpoint_t server_ep;
static coap_message_t request[1];  // This will hold the CoAP request

PROCESS_THREAD(hello_world_process, ev, data)
{
    PROCESS_BEGIN();
    
    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

    PROCESS_END();
}

static void response_handler(coap_message_t *response)
{
    const uint8_t *payload = NULL;
    int len = coap_get_payload(response, &payload);
    printf("Response received: %.*s\n", len, (char *)payload);
}
