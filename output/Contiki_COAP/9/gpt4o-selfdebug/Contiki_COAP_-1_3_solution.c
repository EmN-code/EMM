#include "sys/rtimer.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "net/app-layer/coap/coap-timer.h"
#include "net/app-layer/coap/coap-request-state.h"

#include "contiki.h"
#include <stdio.h>
#include <string.h>

PROCESS(coap_example_client, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client);

static struct etimer et;

PROCESS_THREAD(coap_example_client, ev, data)
{
  PROCESS_BEGIN();

  static coap_endpoint_t server_endpoint;
  static coap_message_t request[1];  // Array definition acts as pointer

  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_endpoint);
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");

  const char *msg = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, strlen(msg));
  
  /* Wait for some time before sending the request */
  etimer_set(&et, CLOCK_SECOND * 2);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

  /* Sending request to the CoAP server */
  COAP_BLOCKING_REQUEST(&server_endpoint, request, NULL);
  
  PROCESS_END();
}

int main(void) {
  process_init();
  process_start(&coap_example_client, NULL);
  while(1) {
    process_run();
  }
  return 0;
}