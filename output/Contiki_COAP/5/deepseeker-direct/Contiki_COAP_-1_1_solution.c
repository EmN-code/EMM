
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "lib/random.h"
#include "sys/etimer.h"
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static struct etimer periodic_timer;
static coap_message_t request;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }
  printf("Response received\n");
}

PROCESS_THREAD(coap_client_process, ev, data)
{
  static coap_endpoint_t server_ep;

  PROCESS_BEGIN();

  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
  server_ep.port = SERVER_PORT;

  while(1) {
    etimer_set(&periodic_timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, MESSAGE, strlen(MESSAGE));

    COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);
  }

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  return 0;
}
