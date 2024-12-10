
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "dev/leds.h"
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static struct etimer et;

/* Define the CoAP request callback */
static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }

  const uint8_t *chunk;
  size_t len = coap_get_payload(response, &chunk);

  if(len > 0) {
    printf("Response received: %.*s\n", (int)len, (char *)chunk);
  }
}

PROCESS(coap_post_process, "CoAP POST Process");
AUTOSTART_PROCESSES(&coap_post_process);

PROCESS_THREAD(coap_post_process, ev, data)
{
  static coap_endpoint_t server_ep;
  static coap_message_t request;

  PROCESS_BEGIN();

  /* Initialize the CoAP endpoint */
  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
  server_ep.port = SERVER_PORT;

  /* Set up the CoAP request */
  coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(&request, URI_PATH);
  coap_set_payload(&request, MESSAGE, strlen(MESSAGE));

  /* Send the CoAP request */
  COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);

  /* Schedule the next request */
  etimer_set(&et, CLOCK_SECOND * 10);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);
    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  /* Initialize Contiki */
  process_init();
  process_start(&coap_post_process, NULL);

  /* Main loop */
  while(1) {
    process_run();
  }

  return 0;
}
