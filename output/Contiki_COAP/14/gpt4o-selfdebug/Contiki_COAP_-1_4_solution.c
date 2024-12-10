#include "contiki.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
    PROCESS_BEGIN();

    static coap_blocking_request_state_t request_state;
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];

    // Define the server endpoint - assuming IPv4 address is applicable
    // Adjusting as needed for IPv6 if on such stack
    uip_ip6addr(&server_ep, 0x3132, 0x3633, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0);
    server_ep.port = UIP_HTONS(5683);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    COAP_BLOCKING_REQUEST(&server_ep, request,
                          (block_handler){
        // The response handler can be defined here
    });

    PROCESS_END();
}

/* Note: Actual IP address setting might vary based on whether IPv6 or IPv4 is available.
   Ensure that your CoAP and network configuration is correctly set to use IPv6 or IPv4 explicitly. */