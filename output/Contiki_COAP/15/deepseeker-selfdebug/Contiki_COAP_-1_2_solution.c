
#include <stdio.h>
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "sys/etimer.h"

#define SERVER_EP "coap://47.102.103.1:5683"
#define URI_PATH "/coap/test"

static struct coap_link_t resource_list[] = {
  { NULL, NULL }
};

static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }

  const uint8_t *payload;
  size_t payload_len;
  if(coap_get_payload(response, &payload, &payload_len)) {
    printf("Response: %.*s\n", (int)payload_len, payload);
  }
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  static struct etimer et;
  static coap_message_t request;
  static coap_endpoint_t server_ep;

  PROCESS_BEGIN();

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  etimer_set(&et, CLOCK_SECOND * 5);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

  coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(&request, URI_PATH);
  coap_set_payload(&request, (uint8_t *)"Hello, COAP Cloud", 16);

  COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);

  PROCESS_END();
}
