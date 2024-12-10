#include "contiki.h"
#include "coap-engine.h"
#include "coap-request-state.h" 
#include "contiki-conf.h"

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer request_timer;
static coap_endpoint_t server_endpoint;
static coap_message_t request;

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&request_timer, CLOCK_SECOND * 5); // Send request every 5 seconds

  // Configure the server endpoint
  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_endpoint);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&request_timer));
    
    // Prepare the request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(&request, "/coap/test");
    coap_set_payload(&request, (uint8_t *)"Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);
    
    // Send the request
    COAP_BLOCKING_REQUEST(&server_endpoint, &request, response_handler);

    // Reset the timer
    etimer_reset(&request_timer);
  }

  PROCESS_END();
}

void response_handler(coap_message_t *response) {
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response received: %.*s\n", len, (char *)chunk);
}

int main() {
  contiki_start();
  return 0;
}