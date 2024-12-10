
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-debug.h"
#include "dev/button-hal.h"
#include "dev/leds.h"
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct etimer et;
static coap_endpoint_t server_ep;
static coap_message_t request;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }

  const uint8_t *payload;
  uint16_t payload_len = coap_get_payload(response, &payload);

  if(payload_len > 0) {
    printf("Response received: %.*s\n", payload_len, payload);
  } else {
    printf("Empty response received\n");
  }
}

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  static coap_endpoint_t server_ep;
  static coap_message_t request;

  // Initialize the CoAP endpoint
  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
  coap_endpoint_set_port(&server_ep, SERVER_PORT);

  // Create a CoAP request
  coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(&request, URI_PATH);
  const char *payload = "Hello, COAP Cloud";
  coap_set_payload(&request, (uint8_t *)payload, strlen(payload));

  // Send the CoAP request
  COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  return 0;
}
