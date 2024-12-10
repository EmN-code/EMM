
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include <string.h>

#define LOG_MODULE "CoAP Client"
#define LOG_LEVEL LOG_LEVEL_INFO

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct coap_endpoint_t server_ep;
static struct coap_request_t request;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    LOG_INFO("No response received\n");
    return;
  }

  char payload[32];
  int len = coap_get_payload(response, (uint8_t *)payload);

  if(len > 0) {
    payload[len] = '\0';
    LOG_INFO("Response received: %s\n", payload);
  }
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static struct etimer et;

  PROCESS_BEGIN();

  LOG_INFO("Starting CoAP client\n");

  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
  server_ep.port = SERVER_PORT;

  while(1) {
    etimer_set(&et, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_init_request(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, (uint8_t *)"Hello, COAP Cloud", 16);

    COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);
  }

  PROCESS_END();
}
