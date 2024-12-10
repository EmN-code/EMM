#include "coap-request-engine.h"
#include "coap-engine.h"
#include "contiki-net.h"
#include "contiki.h"
#include <string.h>

PROCESS(coap_client_example, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_example);

static void coap_post_handler(coap_message_t *response)
{
    const uint8_t *payload = NULL;
    int len = 0;

    len = coap_get_payload(response, &payload);
    printf("Response: %s\n", payload);
}

PROCESS_THREAD(coap_client_example, ev, data)
{
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];
    static char msg[] = "Hello, COAP Cloud";

    PROCESS_BEGIN();
    
    coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, msg, strlen(msg));

    while (1) {
        COAP_BLOCKING_REQUEST(&server_ep, request, coap_post_handler);
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
        
        // Placeholder for delay, e.g., etimer restarted
        // etimer_reset(&et);
    }

    PROCESS_END();
}