#include "sys/log.h"
#include "coap-engine.h"
#include "os/sys/ctimer.h"
#include "net/mac/tsch/tsch.h"
#include "contiki.h"
#include <string.h>

// Set the log module
#define LOG_MODULE "CoAP Client"
#define LOG_LEVEL LOG_LEVEL_INFO

// CoAP server IP and port
#define SERVER_EP "coap://[47.102.103.1]:5683"

// Resource URI
#define POST_RESOURCE_URI "/coap/test"

// Define the maximum payload size
#define MAX_PAYLOAD_LEN 64

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
  const uint8_t *payload = NULL;
  if(response == NULL) {
    LOG_INFO("Request timed out\n");
    return;
  }
  int len = coap_get_payload(response, &payload);
  LOG_INFO("Response: %*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];
  static struct etimer et;

  PROCESS_BEGIN();

  LOG_INFO("Starting CoAP Client\n");

  // Set the server endpoint
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  // Create the request
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, POST_RESOURCE_URI);
  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  // Send request and specify response handler
  COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

  // Wait some time until next request
  etimer_set(&et, CLOCK_SECOND * 10);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

  PROCESS_END();
}