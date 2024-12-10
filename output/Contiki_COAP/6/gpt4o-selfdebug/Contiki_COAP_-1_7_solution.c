#include "net/app-layer/coap/coap-request-state.h"
#include "contiki-default-conf.h"

PROCESS(hello_world_process, "Hello World CoAP Client");
AUTOSTART_PROCESSES(&hello_world_process);

static coap_endpoint_t coap_server_ep;
static coap_message_t request[1]; // Make it an array so memset works
static struct etimer et;

PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  coap_init_engine();

  coap_endpoint_parse("coap://47.102.103.1:5683", &coap_server_ep);

  etimer_set(&et, CLOCK_SECOND * 10);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&et)) {
      // Prepare the CoAP request
      coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
      coap_set_header_uri_path(request, "/coap/test");
      const char msg[] = "Hello, COAP Cloud";
      coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

      // Send the request
      COAP_BLOCKING_REQUEST(&coap_server_ep, request, coap_receive);

      etimer_reset(&et);
    }
  }

  PROCESS_END();
}