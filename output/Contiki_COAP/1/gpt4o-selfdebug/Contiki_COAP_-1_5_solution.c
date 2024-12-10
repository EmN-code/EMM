#include "contiki.h"
#include "coap-engine.h"
#include "sys/etimer.h"
#include "net/ip/uip-debug.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  static coap_message_t request[1];  // Declare the request variable

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&et)) {
      // Initialize CoAP engine
      coap_engine_init();
      
      // Prepare the request message
      coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
      coap_set_header_uri_path(request, COAP_URI_PATH);
      coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

      // Send the request to the server
      uip_ipaddr_t server_ipaddr;
      uip_ipaddr(&server_ipaddr, COAP_SERVER_IP);
      COAP_BLOCKING_REQUEST(&server_ipaddr, UIP_HTONS(COAP_SERVER_PORT), request);

      etimer_reset(&et);  // Reset the timer
    }
  }
  PROCESS_END();
}