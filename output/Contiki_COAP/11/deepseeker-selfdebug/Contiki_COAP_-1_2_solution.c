
#include <stdio.h>
#include "coap-blocking-request.h"
#include "contiki.h"
#include "contiki-net.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "sys/etimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void coap_response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }
  char payload[32];
  int len = coap_get_payload(response, (uint8_t **)&payload);
  if(len > 0) {
    printf("Response received: %s\n", payload);
  }
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static struct etimer et;
  static coap_endpoint_t server_endpoint;
  static coap_message_t request;

  PROCESS_BEGIN();

  etimer_set(&et, CLOCK_SECOND * 5);

  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_endpoint);
  coap_endpoint_set_port(&server_endpoint, SERVER_PORT);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    COAP_BLOCKING_REQUEST(&server_endpoint, &request, coap_response_handler);

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  return 0;
}