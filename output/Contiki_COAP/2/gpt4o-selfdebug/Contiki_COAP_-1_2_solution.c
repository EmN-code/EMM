#include <contiki-net.h> 
#include <contiki.h>

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;

PROCESS_THREAD(coap_client_process, ev, data)
{
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];
    uip_ipaddr_t ipaddr;

    PROCESS_BEGIN();

    // Set server IP and port
    uip_ip6addr(&ipaddr, 0x47, 0x102, 0x103, 0x1, 0, 0, 0, 0);
    coap_endpoint_parse(SERVER_EP, "coap://[47.102.103.1]:5683", 0);

    // Initialize the CoAP request
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", 16);

    // Send Request
    COAP_BLOCKING_REQUEST(&server_ep, request)

    PROCESS_END();
}