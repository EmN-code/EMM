
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

static void
response_handler(coap_message_t *response)
{
  if(response == NULL) {
    printf("Request timed out\n");
    return;
  }

  uint8_t payload[32];
  int len = coap_get_payload(response, payload, sizeof(payload));

  if(len > 0) {
    printf("Response received: %.*s\n", len, payload);
  }
}

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  static coap_endpoint_t server_ep;
  static coap_message_t request;

  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
  coap_endpoint_set_port(&server_ep, SERVER_PORT);

  coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(&request, URI_PATH);
  coap_set_payload(&request, "Hello, COAP Cloud", 16);

  COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  return 0;
}
