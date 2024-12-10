#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "dev/leds.h"

PROCESS(coap_example_client_process, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client_process);

static char *coap_server_ip = "47.102.103.1";
static uint16_t coap_server_port = 5683;
static char *coap_server_uri = "/coap/test";

static void
response_handler(coap_message_t *response)
{
  const uint8_t *payload;
  if(response == NULL) {
    printf("Request timed out\n");
    return;
  }

  int len = coap_get_payload(response, &payload);
  printf("Response: %.*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_example_client_process, ev, data)
{
  static struct etimer et;
  static coap_endpoint_t server_endpoint;
  static coap_message_t request[1];

  PROCESS_BEGIN();
  
  coap_endpoint_parse(coap_server_ip, coap_server_port, &server_endpoint);

  etimer_set(&et, CLOCK_SECOND * 10);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    printf("Sending CoAP POST request to %s%s\n", coap_server_ip, coap_server_uri);
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, coap_server_uri);
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void) {
  process_start(&coap_example_client_process, NULL);
  while(1) {
    process_run();
    etimer_request_poll();
  }
  return 0;
}
