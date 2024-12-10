
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct coap_link_format_s coap_link_format;
static struct coap_resource_t coap_resource;

static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }
  printf("Response received\n");
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
  static struct etimer et;
  static coap_endpoint_t server_endpoint;
  static coap_message_t request;

  PROCESS_BEGIN();

  // Initialize CoAP endpoint
  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_endpoint);
  coap_endpoint_set_port(&server_endpoint, SERVER_PORT);

  // Set up the request
  coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(&request, URI_PATH);
  coap_set_payload(&request, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

  // Send the request
  COAP_BLOCKING_REQUEST(&server_endpoint, &request, response_handler);

  // Wait for a while before sending the next request
  etimer_set(&et, CLOCK_SECOND * 5);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

  PROCESS_END();
}
