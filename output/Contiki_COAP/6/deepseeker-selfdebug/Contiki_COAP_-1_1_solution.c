
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "contiki-net.h"
#include "sys/etimer.h"
#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct coap_endpoint_t server_ep;
static struct coap_packet_t request;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }
  printf("Response received\n");
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static struct etimer et;
  static coap_blocking_request_t blocking;

  PROCESS_BEGIN();

  // Initialize the CoAP endpoint
  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
  server_ep.port = SERVER_PORT;

  // Create a CoAP POST request
  coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(&request, URI_PATH);
  coap_set_payload(&request, "Hello, COAP Cloud", 16);

  // Set up the blocking request
  COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);

  // Send the request every 10 seconds
  etimer_set(&et, CLOCK_SECOND * 10);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    coap_blocking_request(&blocking, &server_ep, &request);
    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  return 0;
}
