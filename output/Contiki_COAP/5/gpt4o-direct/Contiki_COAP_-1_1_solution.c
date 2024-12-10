#include "uip.h"
#include "sys/etimer.h"
#include "rest-engine.h"
#include "dev/button-sensor.h"
#include "contiki.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT UIP_HTONS(5683)
#define URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP client process");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;

static void client_chunk_handler(void *response) {
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  static coap_endpoint_t server_ep;
  static coap_message_t request[1];

  coap_endpoint_parse(SERVER_IP, SERVER_PORT, &server_ep);

  while(1) {
    etimer_set(&et, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, URI_PATH);
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, msg, sizeof(msg) - 1);
    
    printf("Sending request to: %s\n", SERVER_IP);

    COAP_BLOCKING_REQUEST(&server_ep, request, client_chunk_handler);
  }

  PROCESS_END();
}