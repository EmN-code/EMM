#include "contiki.h"
#include "log.h"
#include "string.h"
#include "sys/etimer.h"
#include "net/netstack.h"
#include "coap-engine.h"
#include "coap-request-state.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"
static coap_endpoint_t server_ep;
static coap_message_t request[1];
static struct etimer et;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);
  
  etimer_set(&et, CLOCK_SECOND * 10);

  while(1) {
    PROCESS_YIELD();
    if(etimer_expired(&et)) {
      LOG_INFO("Sending CoAP POST request\n");
      coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
      coap_set_header_uri_path(request, "/coap/test");
      const char msg[] = "Hello, COAP Cloud";
      coap_set_payload(request, msg, sizeof(msg) - 1);
      COAP_BLOCKING_REQUEST(&server_ep, request, NULL);
      etimer_reset(&et);
    }
  }

  PROCESS_END();
}