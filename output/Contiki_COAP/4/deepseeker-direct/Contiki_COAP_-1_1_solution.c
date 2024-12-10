
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "lib/random.h"
#include "sys/etimer.h"
#include <string.h>
#include <stdio.h>

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

  const uint8_t *payload = NULL;
  uint16_t payload_len = 0;
  coap_get_payload(response, &payload);
  payload_len = coap_get_payload_length(response);

  if(payload_len > 0) {
    printf("Response received: %.*s\n", payload_len, payload);
  } else {
    printf("Empty response received\n");
  }
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  static coap_endpoint_t server_ep;
  static coap_message_t request;

  PROCESS_BEGIN();

  /* Initialize the CoAP endpoint */
  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
  coap_endpoint_set_port(&server_ep, SERVER_PORT);

  /* Set up the CoAP request */
  coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(&request, URI_PATH);
  coap_set_payload(&request, (uint8_t *)MESSAGE, strlen(MESSAGE));

  /* Send the CoAP request */
  COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);

  /* Set a timer to send the request periodically */
  etimer_set(&et, CLOCK_SECOND * 5);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);
    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  return 0;
}
