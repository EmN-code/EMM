#include <stdio.h>
#include <sys/log.h>
#include "coap-engine.h"
#include "coap.h"
#include "contiki.h"

#define LOG_MODULE "coap-client"
#define LOG_LEVEL LOG_LEVEL_DBG

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  static struct etimer timer;
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];
  
  PROCESS_BEGIN();

  LOG_INFO("Starting CoAP Client\n");

  // Initialize the CoAP engine
  coap_engine_init();

  // Configure the server endpoint
  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);

  // Prepare the CoAP request
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  const char *msg = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, strlen(msg));

  // Set a timer to trigger the POST request every 10 seconds
  etimer_set(&timer, 10 * CLOCK_SECOND);

  while(1) {
    PROCESS_YIELD();
    if(etimer_expired(&timer)) {
      LOG_INFO("Sending CoAP POST request\n");
      COAP_BLOCKING_REQUEST(&server_ep, request, NULL);
      etimer_reset(&timer);
    }
  }

  PROCESS_END();
}