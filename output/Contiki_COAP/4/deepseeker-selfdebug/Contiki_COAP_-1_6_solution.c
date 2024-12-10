
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "sys/etimer.h"
#include <string.h>
#include <stdio.h>

#define SERVER_EP "coap://47.102.103.1:5683"
#define URI_PATH "coap/test"

static struct etimer et;
static coap_endpoint_t server_ep;
static coap_message_t request[1];

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("Request timed out\n");
    return;
  }

  char payload[64];
  int len = coap_get_payload(response, (uint8_t *)payload);

  if(len > 0) {
    payload[len] = '\0';
    printf("Response received: %s\n", payload);
  }
}

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  while(1) {
    etimer_set(&et, CLOCK_SECOND * 4);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    coap_set_payload(request, payload, strlen(payload));

    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);
  }

  PROCESS_END();
}
