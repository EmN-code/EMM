#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki.h"

PROCESS(coap_client_process, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_process);

// Server details
static uip_ipaddr_t server_ipaddr;
#define SERVER_PORT 5683
#define SERVER_IP "47.102.103.1"
#define URI_PATH "/coap/test"

// CoAP message
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void
set_global_address(void)
{
  uip_ip6addr(&server_ipaddr, 0x2402, 0xdb08, 0x108, 0x2036, 0, 0, 0, 0x0001);
}

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();
  
  set_global_address();

  static coap_packet_t request[1];

  printf("Starting CoAP client\n");

  coap_init_engine();
  
  // Prepare a request object
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, URI_PATH);
  coap_set_payload(request, (uint8_t *)COAP_PAYLOAD, sizeof(COAP_PAYLOAD) - 1);
  
  // Send CoAP request to the server
  COAP_BLOCKING_REQUEST(&server_ipaddr, SERVER_PORT, request, NULL);

  printf("Request sent to COAP server\n");

  PROCESS_END();
}